#ifndef GLOBALS_H
#define GLOBALS_H

#include "FixedFifo.h"
#include "DataStructs.h"

struct GlobalAggregator {

    DataFrame dataFrame;
    FixedFifo<DataFrame, FRAMES_IN_Q> dataFramesFifo;
    Memory memory;
    float initialPressure;
    //uint8_t rocketState : 3;
};

extern GlobalAggregator glob;

#endif