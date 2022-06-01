#include <FS.h>
#include "Globals.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>

GlobalAggregator glob;

Adafruit_BMP085 bmp;

void setup() {

    Serial.begin(74880);
    Serial.setTimeout(10);

    Wire.begin(SDA_PIN, SCL_PIN);
    bmp.begin(3);

    pinMode(SEPAR1_PIN, OUTPUT);
    pinMode(SEPAR2_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(CONT1_PIN, INPUT_PULLUP);
    pinMode(CONT2_PIN, INPUT_PULLUP);

    Serial.println("siema");
    Serial.println((int)sizeof(DataFrame));

    // TODO config!!!


    // LOOPS:
    glob.dataFrame.rocketState = RAIL;
    glob.loops.railLoop();

    glob.dataFrame.rocketState = FLIGHT;
    glob.loops.flightLoop();

    glob.dataFrame.rocketState = FIRST_SEPAR;
    glob.loops.sep1Loop();

    glob.dataFrame.rocketState = SECOND_SEPAR;
    glob.loops.sep2Loop();

    glob.dataFrame.rocketState = GROUND;
    glob.loops.groundLoop();
}

void loop() {
}