#if !defined(INTERVAL_H)
#define INTERVAL_H
#include <Arduino.h>

class Interval
{
public:
    Interval(unsigned long value)
    {
        this->value = value;
        running = false;
        hasTickedFlag = false;
    }

    void start()
    {
        lastTickTime = millis();
        hasTickedFlag = true;
        running = true;
    }

    void stop()
    {
        lastTickTime = false;
        running = false;
    }

    void update()
    {
        if (!running)
        {
            hasTickedFlag = false;
            return;
        }

        auto now = millis();
        if (lastTickTime + value <= now)
        {
            hasTickedFlag = true;
            lastTickTime = now;
        }
    }

    bool hasTicked()
    {
        if (!running)
        {
            hasTickedFlag = false;
            return false;
        }
        if (hasTickedFlag)
        {
            hasTickedFlag = false;
            return true;
        }
        return false;
    }

    bool isRunning()
    {
        return running;
    }

protected:
    unsigned long value;
    bool running;
    unsigned long lastTickTime;
    bool hasTickedFlag;
};

#endif // INTERVAL_H
