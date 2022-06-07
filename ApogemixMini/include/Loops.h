#ifndef LOOPS_H
#define LOOPS_H

#include "Website.h"

class StateLoops {

    Timer pressMeasureTimer;
    Timer ignitionTimer;
    Timer wifiTimer;

    void dataLoop(bool enableFlashWrite);
    void ignitionLoop(bool apogee);

public:

    void railLoop();
    void flightLoop();
    void sep1Loop();
    void sep2Loop();
    void groundLoop();
};

extern StateLoops loops;

#endif