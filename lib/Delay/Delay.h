#if !defined(DELAY_H)
#define DELAY_H
#include <Arduino.h>

class Delay
{
public:
    Delay(unsigned long value)
    {
        this->value = value;
        isRunning = 0;
    }
    void start()
    {
        startTime = millis();
        isRunning = true;
    }
    void update()
    {
        if (!isRunning)
        {
            return;
        }

        if (startTime + value < millis())
        {
            isRunning = false;
            hasExpiredButNotRead = true;
        }
    }
    bool hasExpired()
    {
        if (isRunning)
        {
            return false;
        }
        if (hasExpiredButNotRead)
        {
            hasExpiredButNotRead = false;
            return true;
        }
        return false;
    }

private:
    unsigned long value;
    unsigned long startTime;
    bool isRunning;
    bool hasExpiredButNotRead;
};

#endif // DELAY_H
