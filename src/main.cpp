#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Segments.h>
#include <Delay.h>
#include <Interval.h>
#include <Weather.h>
#include <DS3231.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ClickButton.h>
#include <ApiClient.h>
#include <TimeLib.h>

#define BUTTON_PIN D5
#define DISPLAY_LENGTH 20
#define DISPLAY_HEIGHT 4
#define DISPLAY_LAST_LINE (DISPLAY_HEIGHT - 1)

//Devices
WiFiClient wifiClient;
WiFiManager wifiManager;
LiquidCrystal_I2C lcd(0x27, DISPLAY_LENGTH, DISPLAY_HEIGHT); // set the LCD address to 0x27 for a 16 chars and 2 line display
RTClib RTC;
DS3231 Clock;

//Networking
ApiClient apiClient(wifiClient);

//Interface configuration
Segments segments(&lcd);
ClickButton actionButton(BUTTON_PIN, HIGH);
Delay backlightDelay(10000);
Delay fullResetConfirmationDelay(5000);
Delay restartDelay(1000);
Interval weatherUpdateInterval(15 * 60 * 1000);

//State
unsigned long lastTimeDisplayTime = 0;
Weather currentWeather;

void clearMessage()
{
    for (int i = 0; i < DISPLAY_LENGTH; i++)
    {
        lcd.setCursor(i, DISPLAY_LAST_LINE);
        lcd.print(' ');
    }
}
void showMessage(String message)
{
    uint8_t len = message.length();
    uint8_t spacesLength = DISPLAY_LENGTH - len;
    lcd.setCursor(0, DISPLAY_LAST_LINE);
    lcd.print(message);
    for (int i = 0; i < spacesLength; i++)
    {
        lcd.setCursor(i + len, DISPLAY_LAST_LINE);
        lcd.print(' ');
    }
}

void onWifiAP(WiFiManager *wfmgr)
{
    showMessage("Wifi config mode");
}

void onWifiDone()
{
    showMessage("Wifi configured");
    delay(1000);
    clearMessage();
}

void synchronizeTime(time_t currentTime)
{
    int currentYear = year(currentTime);
    byte currentMonth = month(currentTime);
    byte currentDay = day(currentTime);
    byte currentHour = hour(currentTime);
    byte currentMinute = minute(currentTime);
    byte currentSecond = second(currentTime);

    byte shortYear = currentYear - 2000;

    Clock.setYear(shortYear);
    Clock.setMonth(currentMonth);
    Clock.setDate(currentDay);
    Clock.setHour(currentHour);
    Clock.setMinute(currentMinute);
    Clock.setSecond(currentSecond);
}

void getWeather()
{
    apiClient.getCurrentWeather(currentWeather);
}

void showCurrentWeather()
{
    if (currentWeather.isSuccessful)
    {
        lcd.setCursor(0, 2);
        if (currentWeather.temp > 0)
        {
            lcd.print('+');
        }
        if (currentWeather.temp < 0)
        {
            lcd.print('-');
        }
        lcd.print(currentWeather.temp);
        lcd.print(' ');
        lcd.print(currentWeather.title);
    }
}

void showCurrentTime()
{
    DateTime currentTime = RTC.now();

    byte hour = currentTime.hour();
    byte minute = currentTime.minute();
    byte second = currentTime.second();

    byte digit0 = hour / 10;
    byte digit1 = hour % 10;
    byte digit2 = minute / 10;
    byte digit3 = minute % 10;
    byte digit4 = second / 10;
    byte digit5 = second % 10;

    segments.printChar('0' + digit0, 0, 0);
    segments.printChar('0' + digit1, 4, 0);
    if (second % 2 == 0)
    {
        segments.printChar(':', 8, 0);
    }
    else
    {
        segments.printChar(' ', 8, 0);
    }

    segments.printChar('0' + digit2, 10, 0);
    segments.printChar('0' + digit3, 14, 0);

    lcd.setCursor(17, 1);
    lcd.print(digit4);
    lcd.setCursor(18, 1);
    lcd.print(digit5);
}

void loadWeather()
{
    Serial.println("Getting weather");
    showMessage("Getting weather");
    apiClient.getCurrentWeather(currentWeather);
    showCurrentWeather();
    clearMessage();
    Serial.println("Got weather");
}

void setup()
{
    Serial.begin(9600);
    actionButton.multiclickTime = 200;
    lcd.init();
    lcd.noBacklight();

    segments.initChars();

    showMessage("Connecting to WIFI");
    wifiManager.setDebugOutput(true);
    wifiManager.setConfigPortalTimeout(120);
    wifiManager.setAPCallback(onWifiAP);
    wifiManager.setSaveConfigCallback(onWifiDone);
    bool hasConnected = wifiManager.autoConnect();
    if (hasConnected)
    {
        showMessage("Connected");
    }
    else
    {
        showMessage("Not connected");
    }

    showMessage("Getting time");
    auto currentTime = apiClient.getCurrentTime();
    if (currentTime != 0)
    {
        synchronizeTime(currentTime);
    }
    clearMessage();

    weatherUpdateInterval.start();
}

void loop()
{
    actionButton.Update();
    backlightDelay.update();
    restartDelay.update();
    fullResetConfirmationDelay.update();
    weatherUpdateInterval.update();

    if (millis() - lastTimeDisplayTime > 500)
    {
        showCurrentTime();
        lastTimeDisplayTime = millis();
    }

    if (actionButton.clicks == 3)
    {
        showMessage("Restarting");
        restartDelay.start();
        backlightDelay.start();
    }

    if (actionButton.clicks == 7)
    {
        backlightDelay.start();
        fullResetConfirmationDelay.start();
        showMessage("Full reset? 2cl=YES");
    }
    if (fullResetConfirmationDelay.isRunning() && actionButton.clicks == 2)
    {
        showMessage("Resetting");
        wifiManager.resetSettings();
        delay(1000);
        ESP.restart();
    }
    if (fullResetConfirmationDelay.hasExpired())
    {
        clearMessage();
    }

    //----

    if (actionButton.clicks == 1)
    {
        backlightDelay.start();
    }
    if (backlightDelay.hasStarted())
    {
        lcd.backlight();
    }
    if (backlightDelay.hasExpired())
    {
        lcd.noBacklight();
    }
    if (restartDelay.hasExpired())
    {
        ESP.restart();
    }

    //----

    if (weatherUpdateInterval.hasTicked())
    {
        loadWeather();
    }
}
