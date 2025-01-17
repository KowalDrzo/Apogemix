#include "Loops.h"

GlobalAggregator glob;

void setup() {

    Serial.begin(115200);
    Serial.setTimeout(10);

    delay(3000);
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    // CONFIG:
    EEPROM.begin(sizeof(glob.memory));
    EEPROM.get(0, glob.memory);

    // Only for first programming:
    //tasks.clearMem();

    tasks.readFlash();
    tasks.recalibrate();
    Serial.println(glob.initialTemper);

    // RTOS queues:
    glob.dataFramesFifo = xQueueCreate(FRAMES_IN_Q, sizeof(DataFrame));

    // Pararell tasks:
    xTaskCreate((TaskFunction_t) StateLoops::loraLoop, "LoRa Task",  16384, NULL, 2, NULL);

    // LOOPS:
    loops.railLoop();
}

void loop() {

    vTaskDelay(1 / portTICK_PERIOD_MS);
}
