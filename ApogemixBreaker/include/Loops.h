#ifndef LOOPS_H
#define LOOPS_H

#include <LoRa.h>
#include "Tasks.h"

class StateLoops {

    Timer pressMeasureTimer;
    Timer waitingTimer;
    Timer wifiTimer;

public:

    static void gpsLoop();
    static void loraLoop();
};

extern StateLoops loops;

#endif