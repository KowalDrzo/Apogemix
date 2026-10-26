#ifndef GLOBALS_H
#define GLOBALS_H

#include "LoraModule.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <set>

struct Memory {
    uint16_t frequencyMHz;
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
