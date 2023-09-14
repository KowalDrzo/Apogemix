#include "LoraModule.h"

void LoraModule::init(uint16_t freqMHz) {

    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, LORA_CS_PIN);
    LoRa.setPins(LORA_CS_PIN, LORA_RES_PIN, LORA_D0_PIN);
    LoRa.setSignalBandwidth(125E3);
    LoRa.begin(freqMHz * 1E6);
    LoRa.setTimeout(100);
}

/*********************************************************************/

void LoraModule::send(String txString) {

    LoRa.beginPacket();
    LoRa.println(txString);
    LoRa.endPacket();
}

/*********************************************************************/

String LoraModule::read() {

    if (LoRa.parsePacket()) {
        while (LoRa.available()) {

            digitalWrite(LED_PIN, !digitalRead(LED_PIN));
            return LoRa.readString();
        }
    }
    return String("");
}
