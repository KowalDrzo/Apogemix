#include "Website.h"

GlobalAggregator glob;
Website website;

void setup() {

    Serial.begin(115200);
    Serial.setTimeout(10);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // CONFIG:
    EEPROM.begin(sizeof(glob.memory));
    EEPROM.get(0, glob.memory);
    prepareMem();

    glob.loraModule.init(glob.memory.frequencyMHz);
}

void loop() {

    // USB to LoRa:
    if (Serial.available()) {

        glob.txString = Serial.readString();
        glob.loraModule.send(glob.txString);
        checkForNewFrequency(glob.txString);
    }

    // WiFi to LoRa:
    if (glob.txRequest) {

        glob.txRequest = false;
        glob.loraModule.send(glob.txString);
    }

    // LoRa to USB:
    String rxString = glob.loraModule.read();
    if (rxString.length() > 0) {
        Serial.print(rxString);
        website.sendWsString(rxString);
    }

    // Website:
    if (!digitalRead(BUTTON_PIN) && !website.isEnabled()) {

        SPIFFS.begin(true);
        website.start();
    }

    vTaskDelay(1);
}
