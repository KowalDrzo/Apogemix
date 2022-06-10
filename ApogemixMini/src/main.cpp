#include "Loops.h"

GlobalAggregator glob;

void setup() {

    Serial.begin(74880);
    Serial.setTimeout(10);

    Wire.begin(SDA_PIN, SCL_PIN);
    tasks.bmp.begin(3);

    pinMode(SEPAR1_PIN, OUTPUT);
    pinMode(SEPAR2_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(CONT1_PIN, INPUT_PULLUP);
    pinMode(CONT2_PIN, INPUT_PULLUP);

    // CONFIG:
    EEPROM.begin(sizeof(glob.memory));
    EEPROM.get(0, glob.memory);

    // Only for first programming:
    //tasks.clearMem();

    tasks.readFlash();
    glob.initialPressure = tasks.bmp.readPressure();
    tasks.buzz();

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

    delay(1000);
    Serial.println("Waiting for pickup");
}