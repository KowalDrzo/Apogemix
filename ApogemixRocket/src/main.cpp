#include "Loops.h"

GlobalAggregator glob;

void setup() {

    Serial.begin(115200);
    Serial.setTimeout(10);

    delay(1000);

    pinMode(SEPAR1_PIN, OUTPUT);
    pinMode(SEPAR2_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(CONT1_PIN, INPUT);
    pinMode(CONT2_PIN, INPUT);
    pinMode(CONT3_PIN, INPUT);

    // CONFIG:
    EEPROM.begin(sizeof(glob.memory));
    EEPROM.get(0, glob.memory);

    // Only for first programming:
    //tasks.clearMem();

    // RTOS queues:
    glob.dataFramesFifo = xQueueCreate(FRAMES_IN_Q, sizeof(DataFrame));

    // Pararell tasks:
    xTaskCreate((TaskFunction_t) StateLoops::loraLoop, "LoRa Task",  16384, NULL, 2, NULL);

    // LOOPS:
    glob.dataFrame.rocketState = RAIL;
    Serial.println("RAIL STATE");
    loops.railLoop();
}

void loop() {

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
