#ifndef GLOBALS_H
#define GLOBALS_H

#include "LoraModule.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <map>

struct GlobalAggregator {

    uint16_t frequencyMHz;
    String rxString;
    String txString;
    LoraModule loraModule;
    std::map<String, String> deviceDict;
    bool txRequest;
};

extern GlobalAggregator glob;

#endif