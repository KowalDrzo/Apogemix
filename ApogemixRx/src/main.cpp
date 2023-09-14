#include <Functions.h>

GlobalAggregator glob;

void setup() {

    Serial.begin(115200);
    Serial.setTimeout(10);
    pinMode(LED_PIN, OUTPUT);

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
    Serial.print(glob.loraModule.read());

    vTaskDelay(1);
}
