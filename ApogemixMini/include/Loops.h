#ifndef LOOPS_H
#define LOOPS_H

#include "Website.h"

class StateLoops {

    Timer pressMeasureTimer;
    Timer waitingTimer;
    Timer wifiTimer;

    void dataLoop(bool enableFlashWrite);
    void ignitionLoop(bool apogee);
    void waitAndLogData(uint32_t time_ms);

public:

    void railLoop();
    void flightLoop();
    void sep1Loop();
    void sep2Loop();
    void groundLoop();
};

extern StateLoops loops;

#endif