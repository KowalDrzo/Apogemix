#ifndef GLOBALS_H
#define GLOBALS_H

#include "DataStructs.h"

struct GlobalAggregator {

    DataFrame dataFrame;
    QueueHandle_t dataFramesFifo;
    Memory memory;
    float initialPressure;
    float initialTemper;
    int16_t apogee = 0;
    int16_t maxSpeed = 0;
};

extern GlobalAggregator glob;

#endif