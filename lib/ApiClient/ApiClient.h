#if !defined(API_CLIENT_H)
#define API_CLIENT_H

#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

class ApiClient
{
public:
    ApiClient(Client &networkClient)
    {
        this->worldClockApi = new HttpClient(networkClient, "worldtimeapi.org");
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

private:
    HttpClient *worldClockApi;
};

#endif // API_CLIENT_H
