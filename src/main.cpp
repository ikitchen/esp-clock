#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Segments.h>
#include <DS3231.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ClickButton.h>

#define BUTTON_PIN D5
#define DISPLAY_LENGTH 20
#define DISPLAY_HEIGHT 4

WiFiManager wifiManager;
LiquidCrystal_I2C lcd(0x27, DISPLAY_LENGTH, DISPLAY_HEIGHT); // set the LCD address to 0x27 for a 16 chars and 2 line display
Segments segments(&lcd);
RTClib RTC;
ClickButton actionButton(BUTTON_PIN, HIGH);

void clearMessage()
{
  lcd.setCursor(0, 3);
  lcd.print("                    ");
}
void showMessage(String message)
{
  uint8_t len = message.length();
  uint8_t spacesLength = DISPLAY_LENGTH - len;
  lcd.setCursor(0, 3);
  lcd.print(message);
  for (int i = 0; i < spacesLength; i++)
  {
    lcd.setCursor(i + len, 3);
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

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  segments.initChars();

  showMessage("Connecting to WIFI");
  wifiManager.setTimeout(2);
  wifiManager.setConfigPortalTimeout(300);
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
}

void loop()
{
  actionButton.Update();
  segments.printChar('2', 0, 0);
  segments.printChar('3', 4, 0);
  segments.printChar('5', 8, 0);
  segments.printChar('7', 12, 0);
  lcd.setCursor(0, 2);

  DateTime now = RTC.now();
  lcd.print(now.year(), DEC);

  if (actionButton.clicks != 0)
  {
    showMessage(String(actionButton.clicks));
  }
}
