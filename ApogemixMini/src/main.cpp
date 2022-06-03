#include <FS.h>
#include "Loops.h"
#include <ESP8266WiFi.h>

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

    // TODO config!!!
    glob.initialPressure = tasks.bmp.readPressure();

    // LOOPS:
    glob.dataFrame.rocketState = RAIL;
    loops.railLoop();

    glob.dataFrame.rocketState = FLIGHT;
    loops.flightLoop();

    glob.dataFrame.rocketState = FIRST_SEPAR;
    loops.sep1Loop();

    glob.dataFrame.rocketState = SECOND_SEPAR;
    loops.sep2Loop();

    glob.dataFrame.rocketState = GROUND;
    loops.groundLoop();
}

void loop() {
}