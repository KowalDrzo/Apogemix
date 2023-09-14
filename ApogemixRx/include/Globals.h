#ifndef GLOBALS_H
#define GLOBALS_H

#include "LoraModule.h"

struct GlobalAggregator {

    uint16_t frequencyMHz;
    String rxString;
    String txString;
    LoraModule loraModule;
};

extern GlobalAggregator glob;

#endif