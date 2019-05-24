#if !defined(API_CLIENT_H)
#define API_CLIENT_H

#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <Weather.h>

#define TIME_API_HOST "worldtimeapi.org"
#define WEATHER_API_HOST "api.openweathermap.org"
#define WEATHER_API_KEY "0535e24aa935ed754ad31116bffecd4c"
#define WEATHER_API_UNITS "metric"
#define WEATHER_API_Q "Kharkiv"

class ApiClient
{
public:
    ApiClient(Client &networkClient)
    {
        this->worldClockApi = new HttpClient(networkClient, TIME_API_HOST);
        this->openWeatherMapApi = new HttpClient(networkClient, WEATHER_API_HOST);
    }

    time_t getCurrentTime()
    {
        worldClockApi->get("/api/ip");
        if (worldClockApi->responseStatusCode() != 200)
        {
            return 0;
        }
        String response = worldClockApi->responseBody();

        const size_t capacity = JSON_OBJECT_SIZE(14) + 320;
        DynamicJsonDocument doc(capacity);

        deserializeJson(doc, response);
        time_t unixtime = doc["unixtime"];
        unsigned int rawOffset = doc["raw_offset"];
        unsigned int dstOffset = doc["dst_offset"];

        return unixtime + rawOffset + dstOffset;
    }

    void getCurrentWeather(Weather &weatherStruct)
    {
        openWeatherMapApi->get("/data/2.5/weather?appid=" WEATHER_API_KEY "&units=" WEATHER_API_UNITS "&q=" WEATHER_API_Q);
        if (openWeatherMapApi->responseStatusCode() != 200)
        {
            weatherStruct.isSuccessful = false;
            return;
        }
        String response = openWeatherMapApi->responseBody();
        const size_t capacity = JSON_ARRAY_SIZE(1) +
                                JSON_OBJECT_SIZE(1) +
                                2 * JSON_OBJECT_SIZE(2) +
                                JSON_OBJECT_SIZE(4) +
                                JSON_OBJECT_SIZE(5) +
                                JSON_OBJECT_SIZE(6) +
                                JSON_OBJECT_SIZE(13) +
                                270;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);
        JsonObject main = doc["main"];
        int main_temp = main["temp"];

        JsonObject weather_0 = doc["weather"][0];
        const char *weather_0_main = weather_0["main"];

        weatherStruct.isSuccessful = true;
        weatherStruct.temp = main_temp;
        weatherStruct.title = weather_0_main;

        Serial.println("main_temp");
        Serial.println(main_temp);
        Serial.println("weather_0_main");
        Serial.println(weather_0_main);
        Serial.println(response);
    }

private:
    HttpClient *worldClockApi;
    HttpClient *openWeatherMapApi;
};

#endif // API_CLIENT_H
