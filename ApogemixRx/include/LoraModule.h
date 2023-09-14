#ifndef LORA_MODULE_H
#define LORA_MODULE_H

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

class LoraModule {

public:

    void init();
    void setFrequency(uint16_t freqMHz);
    void send(String txString);
    
    bool available();
    String read();
};

#endif