#ifndef LOOPS_H
#define LOOPS_H

#include "Tasks.h"

class StateLoops {

    Timer pressMeasureTimer;

public:

    void railLoop();
    void flightLoop();
    void sep1Loop();
    void sep2Loop();
    void groundLoop();
};

extern StateLoops loops;

#endif