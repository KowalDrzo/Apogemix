#ifndef GLOBALS_H
#define GLOBALS_H

#include "FixedFifo.h"
#include "DataStructs.h"

struct GlobalAggregator {

    DataFrame dataFrame;
    FixedFifo<DataFrame, FRAMES_IN_Q> dataFramesFifo;
    Memory memory;
    float initialPressure;
    float initialTemper;
    int16_t apogee = 0;
    int16_t maxSpeed = 0;
    uint8_t forceRecoveryTest = 0;
    bool isSep1BeforeApog = false;
};

extern GlobalAggregator glob;

#endif