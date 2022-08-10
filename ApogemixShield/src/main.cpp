/*#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

Adafruit_BMP280 bmp(&Wire);
float initialPressure;

void setup() {

    Wire.begin(21, 22, 100000);

    Serial.begin(115200);
    Serial.print("AAA  ");
    Serial.println(bmp.begin(0x76));
    bmp.takeForcedMeasurement();

    initialPressure = bmp.readPressure();
}

void loop() {

    uint32_t tim1 = micros();
    float press = bmp.readPressure();
    float alt = 44330*(1.0 - pow(press/initialPressure, 0.1903));
    tim1 = micros() - tim1;
    Serial.printf("%0.1f   %d\n", alt, (int)tim1);

    delay(200);
}

*/

#include "Loops.h"

GlobalAggregator glob;

void setup() {

    Serial.begin(115200);
    Serial.setTimeout(10);

    Wire.begin(SDA_PIN, SCL_PIN);
    tasks.bmp.begin(0x76);

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

    digitalWrite(BUZZER_PIN, 1);
    Serial.println("Waiting for pickup");
    delay(1000);
    digitalWrite(BUZZER_PIN, 0);
    delay(9000);
}