#ifndef LORA_MODULE_H
#define LORA_MODULE_H

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "PinDefinitions.h"

class LoraModule {

public:

    void init(uint16_t freqMHz);
    void setFrequency(uint16_t freqMHz) { LoRa.setFrequency(freqMHz * 1E6); }
    void send(String txString);

    String read();
};

#endif