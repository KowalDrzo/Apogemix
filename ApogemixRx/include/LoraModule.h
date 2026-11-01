#ifndef LORA_MODULE_H
#define LORA_MODULE_H

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "PinDefinitions.h"

class LoraModule {

public:

    void init(float freqMHz);
    void setFrequency(float freqMHz) { LoRa.setFrequency(static_cast<long>(freqMHz * 1000000.0f)); }
    void send(String txString);

    String read();
};

#endif
