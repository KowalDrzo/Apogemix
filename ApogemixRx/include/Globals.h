#ifndef GLOBALS_H
#define GLOBALS_H

#include "LoraModule.h"
#include <EEPROM.h>
#include <LITTLEFS.h>
#include <WiFi.h>

struct GlobalAggregator {

    uint16_t frequencyMHz;
    String rxString;
    String txString;
    LoraModule loraModule;
};

extern GlobalAggregator glob;

#endif