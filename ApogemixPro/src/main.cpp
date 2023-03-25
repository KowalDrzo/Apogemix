#include "Loops.h"

GlobalAggregator glob;

void setup() {

    Serial.begin(115200);
    Serial.setTimeout(10);

    delay(1000);

    Serial1.begin(9600, SERIAL_8N1, GPS_TX_PIN, GPS_RX_PIN);

    Wire.begin(SDA_PIN, SCL_PIN);
    tasks.bmp.begin(BMP085_ULTRAHIGHRES, &Wire);

    pinMode(SEPAR1_PIN, OUTPUT);
    pinMode(SEPAR2_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(CONT1_PIN, INPUT_PULLUP);
    pinMode(CONT2_PIN, INPUT_PULLUP);
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
    xTaskCreate((TaskFunction_t) StateLoops::gpsLoop,  "GPS Task",   4096,  NULL, 2, NULL);
    xTaskCreate((TaskFunction_t) StateLoops::loraLoop, "LoRa Task",  16384, NULL, 2, NULL);
    xTaskCreate((TaskFunction_t) Tasks::flashTask,     "Flash Task", 16384, NULL, 1, NULL);

    // LOOPS:
    glob.dataFrame.rocketState = RAIL;
    Serial.println("RAIL STATE");
    loops.railLoop();

    glob.dataFrame.rocketState = FLIGHT;
    Serial.println("FLIGHT STATE");
    loops.flightLoop();

    glob.dataFrame.rocketState = FIRST_SEPAR;
    tasks.updateDataBase();
    Serial.println("FIRST SEPAR STATE");
    loops.sep1Loop();

    glob.dataFrame.rocketState = SECOND_SEPAR;
    Serial.println("SECOND SEPAR STATE");
    loops.sep2Loop();

    glob.dataFrame.rocketState = GROUND;
    Serial.println("GROUND STATE");
    loops.groundLoop();
}

void loop() {

    vTaskDelay(1 / portTICK_PERIOD_MS);
}