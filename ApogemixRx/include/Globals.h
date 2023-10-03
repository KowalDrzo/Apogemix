#ifndef GLOBALS_H
#define GLOBALS_H

#include "LoraModule.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <set>

struct GlobalAggregator {

    uint16_t frequencyMHz;
    String rxString;
    String txString;
    LoraModule loraModule;
    std::set<String> deviceDict;
    bool txRequest;
};

extern GlobalAggregator glob;

#endif