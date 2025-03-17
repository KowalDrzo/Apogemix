#include "Tasks.h"

Tasks tasks;

#define CRITERIA_MARGIN 5

void Tasks::continuityTest() {

    glob.dataFrame.continuity1 = !digitalRead(CONT1_PIN);
    glob.dataFrame.continuity2 = !digitalRead(CONT2_PIN);
}

/*********************************************************************/

float Tasks::getPressureMedian() {

    float press[3];

    for (uint8_t i = 0; i < 3; i++) {
        press[i] = bmp.readPressure();
    }

    std::sort(press, press+3);
    return press[1];
}

/*********************************************************************/

void Tasks::measure() {

    // Pressure and temperature:
    glob.dataFrame.pressure = getPressureMedian();
    glob.dataFrame.temper = bmp.readTemperature() * TEMPERATURE_FIX_A + TEMPERATURE_FIX_B;

    // Time:
    uint32_t newTime = millis();
    uint32_t timeDiff = newTime - glob.dataFrame.time;
    if (!timeDiff) timeDiff++;
    glob.dataFrame.time = newTime;

    // Altitude:
    float oldAlt = glob.dataFrame.altitude;
    float newAlt = 44330*(1.0 - pow(glob.dataFrame.pressure/glob.initialPressure, 0.1903));
    glob.dataFrame.altitude = ALPHA_H * oldAlt + (1-ALPHA_H) * newAlt;

    // Speed:
    float oldSpd = glob.dataFrame.speed;
    float newSpd = 1000.0*(glob.dataFrame.altitude - oldAlt) / timeDiff;
    glob.dataFrame.speed = ALPHA_V * oldSpd + (1-ALPHA_V) * newSpd;

    // Contunuity:
    continuityTest();

    // Maximas:
    if (glob.apogee < glob.dataFrame.altitude) glob.apogee = glob.dataFrame.altitude;
    if (glob.maxSpeed < glob.dataFrame.speed) glob.maxSpeed = glob.dataFrame.speed;

    // Debug:
    Serial.println(glob.dataFrame.toString());
}

/*********************************************************************/

void Tasks::buzzBeep(uint16_t activeTime, uint16_t sleepTime, uint8_t n) {

    for (; n > 0; n--) {

        digitalWrite(BUZZER_PIN, 1);
        delay(activeTime);
        digitalWrite(BUZZER_PIN, 0);
        delay(sleepTime);
    }
}

/*********************************************************************/

void Tasks::buzz() {

    continuityTest();

    buzzBeep(30, 150, 2);
    delay(2000);

    if (glob.dataFrame.continuity1 && glob.dataFrame.continuity2)   buzzBeep(500, 500, 3);
    else if (glob.dataFrame.continuity1)                            buzzBeep(500, 500, 1);
    else if (glob.dataFrame.continuity2)                            buzzBeep(500, 500, 2);
}

/*********************************************************************/

bool Tasks::isLaunchDetected() {

    if (glob.dataFrame.altitude > 20 && glob.dataFrame.speed > 1) {

        criteriaCounter++;
        if (criteriaCounter >= CRITERIA_MARGIN) {

            criteriaCounter = 0;
            return true;
        }
    }
    else criteriaCounter = 0;

    return false;
}

/*********************************************************************/

bool Tasks::isApogeeDetected() {

    if (glob.dataFrame.speed < -1 || (glob.isSep1BeforeApog && glob.dataFrame.speed < 10)) {

        criteriaCounter++;
        if (criteriaCounter >= CRITERIA_MARGIN) {

            criteriaCounter = 0;
            return true;
        }
    }
    else criteriaCounter = 0;

    return false;
}

/*********************************************************************/

bool Tasks::isSecondChuteTime() {

    if (glob.dataFrame.altitude < glob.memory.secondSeparAltitude) {

        criteriaCounter++;
        if (criteriaCounter >= CRITERIA_MARGIN) {

            criteriaCounter = 0;
            return true;
        }
    }
    else criteriaCounter = 0;

    return false;
}

/*********************************************************************/

bool Tasks::isOnGround() {

    if (abs(glob.dataFrame.speed) < 2) {

        criteriaCounter++;
        if (criteriaCounter >= CRITERIA_MARGIN) {

            criteriaCounter = 0;
            return true;
        }
    }
    else criteriaCounter = 0;

    return false;
}

/*********************************************************************/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void Tasks::writeToFlash(bool force) {

    if (!appendFlash) {

        appendFlash = true;
        file = LittleFS.open("/FlightData.apg", "w");
    }
    else {
        file = LittleFS.open("/FlightData.apg", "a");
    }

    if (glob.dataFramesFifo.size() > 20 || force) {

        while(glob.dataFramesFifo.size()) {

            DataFrame tempData = glob.dataFramesFifo.pop();
            char tempDataAscii[120];
            strcpy(tempDataAscii, tempData.toString().c_str());
            strcat(tempDataAscii, "\n");
            file.write((uint8_t*) tempDataAscii, strlen(tempDataAscii));
        }
    }

    file.close();
}

/*********************************************************************/

void Tasks::readFlash() {

    LittleFS.begin(true);

    file = LittleFS.open("/FlightData.apg", "r");

    while (file.available()) Serial.print(file.readString());

    file.close();
}

#pragma GCC diagnostic pop

/*********************************************************************/

void Tasks::clearMem() {

    LittleFS.begin(true);
    File file = LittleFS.open("/FlightData.apg", "w");

    for (uint8_t i = 0; i < FLIGHTS_IN_MEM; i++) {

        glob.memory.lastFlightIndex = 0;
        glob.memory.lastFlightNum = 0;
        glob.memory.flight[i].num = 0;
        glob.memory.flight[i].apogee = 0;
        glob.memory.flight[i].maxSpeed = 0;
        glob.memory.secondSeparAltitude = 100;
    }

    EEPROM.put(0, glob.memory);
    EEPROM.commit();

    buzzBeep(30, 30, 10);
    while(1) {
        delay(1000);
        Serial.println("Cleared");
    }
}

/*********************************************************************/

void Tasks::updateDataBase() {

    glob.memory.lastFlightIndex++;
    if (glob.memory.lastFlightIndex > FLIGHTS_IN_MEM) glob.memory.lastFlightIndex = 0;
    uint8_t flid = glob.memory.lastFlightIndex;

    glob.memory.lastFlightNum++;

    glob.memory.flight[flid].num = glob.memory.lastFlightNum;
    glob.memory.flight[flid].apogee = glob.apogee;
    glob.memory.flight[flid].maxSpeed = glob.maxSpeed;

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
}

/*********************************************************************/

void Tasks::recalibrate() {

    glob.initialPressure = bmp.readPressure();
    glob.initialTemper = bmp.readTemperature() * TEMPERATURE_FIX_A + TEMPERATURE_FIX_B;
}
