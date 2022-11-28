#include "Loops.h"

DataFrame dataFrame;

void setup() {

    Serial.begin(115200);
    Serial.setTimeout(10);

    Serial1.begin(9600, SERIAL_8N1, 34, 12);

    Serial.println("TESESTTS");
    // Pararell tasks:
    xTaskCreate((TaskFunction_t) StateLoops::gpsLoop,  "GPS Task",  4096,  NULL, 2, NULL);
    xTaskCreate((TaskFunction_t) StateLoops::loraLoop, "LoRa Task", 16384, NULL, 2, NULL);
}

void loop() {

    vTaskDelay(1 / portTICK_PERIOD_MS);
}