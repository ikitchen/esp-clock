#if !defined(DELAY_H)
#define DELAY_H
#include <Arduino.h>

class Delay
{
public:
    Delay(unsigned long value)
    {
        this->value = value;
        _isRunning = 0;
    }
    void start()
    {
        startTime = millis();
        _isRunning = true;
        hasExpiredFlag = false;
        hasStartedFlag = true;
    }
    void update()
    {
        if (!_isRunning)
        {
            return;
        }

        if (startTime + value < millis())
        {
            _isRunning = false;
            hasExpiredFlag = true;
            hasStartedFlag = false;
        }
    }
    bool hasExpired()
    {
        if (_isRunning)
        {
            return false;
        }
        if (hasExpiredFlag)
        {
            hasExpiredFlag = false;
            return true;
        }
        return false;
    }
    bool hasStarted()
    {
        if (!_isRunning)
        {
            return false;
        }
        if (hasStartedFlag)
        {
            hasStartedFlag = false;
            return true;
        }
        return false;
    }
    bool isRunning()
    {
        return _isRunning;
    }

private:
    unsigned long value;
    unsigned long startTime;
    bool _isRunning;
    bool hasExpiredFlag;
    bool hasStartedFlag;
};

#endif // DELAY_H
