#ifndef LOOPS_H
#define LOOPS_H

#include "Website.h"
#include <LoRa.h>

class StateLoops {

    Timer pressMeasureTimer;
    Timer waitingTimer;
    Timer wifiTimer;

    String rxDebugString;

    void dataLoop(bool enableFlashWrite);
    void ignitionLoop(bool apogee);
    void waitAndLogData(uint32_t time_ms);
    static void loraRxCallback(String rxFrame);

public:

    void railLoop();
    void flightLoop();
    void sep1Loop();
    void sep2Loop();
    void groundLoop();

    static void gpsLoop();
    static void loraLoop();
};

extern StateLoops loops;

#endif