#include "Website.h"

GlobalAggregator glob;
Website website;

void setup() {

    Serial.begin(115200);
    Serial.setTimeout(10);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // CONFIG:
    EEPROM.begin(sizeof(glob.frequencyMHz));
    EEPROM.get(0, glob.frequencyMHz);

    // Only for first programming:
    //clearMem();

    glob.loraModule.init(glob.frequencyMHz);
}

void loop() {

    // USB to LoRa:
    if (Serial.available()) {

        glob.txString = Serial.readString();
        glob.loraModule.send(glob.txString);
        checkForNewFrequency(glob.txString);
    }

    // LoRa to USB:
    String rxString = glob.loraModule.read();
    Serial.print(rxString);
    // TODO WEBSITE SEND rxString

    // Website:
    if (!digitalRead(BUTTON_PIN)) {

        website.start();
    }


    vTaskDelay(1);
}
