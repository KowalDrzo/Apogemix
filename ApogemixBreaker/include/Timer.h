#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {

    uint32_t startTimeStamp;
    uint32_t delayTime_ms;

public:

    void start(uint32_t timeLeft_ms) {

        startTimeStamp = millis();
        delayTime_ms = timeLeft_ms;
    }

    bool check() {

        if (millis() - startTimeStamp >= delayTime_ms) {

            startTimeStamp = millis();
            return true;
        }
        else return false;
    }
};

#endif