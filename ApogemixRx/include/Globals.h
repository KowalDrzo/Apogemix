#ifndef GLOBALS_H
#define GLOBALS_H

#include "LoraModule.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <set>

#define MIN_FREQUENCY_MHZ 410.0f
#define MAX_FREQUENCY_MHZ 490.0f
#define APOGEMIX_ISM_FREQUENCY_MHZ 434.625f
#define APOGEMIX_HAM_FREQUENCY_MHZ 439.000f

struct Memory {
    float frequencyMHz;
    uint16_t initializationMagic;
};

struct GlobalAggregator {

    Memory memory;
    String rxString;
    String txString;
    LoraModule loraModule;
    bool txRequest;
};

extern GlobalAggregator glob;

#endif
