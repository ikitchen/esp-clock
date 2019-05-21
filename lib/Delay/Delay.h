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
            hasExpiredButNotRead = true;
        }
    }
    bool hasExpired()
    {
        if (_isRunning)
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
    bool isRunning()
    {
        return _isRunning;
    }

private:
    unsigned long value;
    unsigned long startTime;
    bool _isRunning;
    bool hasExpiredButNotRead;
};

#endif // DELAY_H
