#include "Tasks.h"

Tasks tasks;

#define CRITERIA_MARGIN 5

void Tasks::measure() {

    int16_t oldAlt = glob.dataFrame.altitude;

    glob.dataFrame.pressure = bmp.readPressure();
    glob.dataFrame.altitude = 44330*(1.0 - pow(glob.dataFrame.pressure/glob.initialPressure, 0.1903));
    
    glob.dataFrame.speed = 1000.0*(glob.dataFrame.altitude - oldAlt)/(millis() - glob.dataFrame.time);

    glob.dataFrame.continuity1 = !digitalRead(CONT1_PIN);
    glob.dataFrame.continuity2 = !digitalRead(CONT2_PIN);

    glob.dataFrame.time = millis();

    if (glob.apogee < glob.dataFrame.altitude) glob.apogee = glob.dataFrame.altitude;
    if (glob.maxSpeed < glob.dataFrame.speed) glob.maxSpeed = glob.dataFrame.speed;

    Serial.printf("%d;%d\n", glob.dataFrame.altitude, glob.dataFrame.speed);
}

/*********************************************************************/

bool Tasks::isLaunchDetected() {

    if (glob.dataFrame.altitude > 10) {

        criteriaCounter++;
        if (criteriaCounter > CRITERIA_MARGIN) {

            criteriaCounter = 0;
            return true;
        } 
    }
    else criteriaCounter = 0;

    return false;
}

/*********************************************************************/

bool Tasks::isApogeeDetected() {

    if (glob.dataFrame.speed < -1) {

        criteriaCounter++;
        if (criteriaCounter > CRITERIA_MARGIN) {

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
        if (criteriaCounter > CRITERIA_MARGIN) {

            criteriaCounter = 0;
            return true;
        }
    }
    else criteriaCounter = 0;

    return false;
}

bool Tasks::isOnGround() {

    if (abs(glob.dataFrame.speed) < 2) {

        criteriaCounter++;
        if (criteriaCounter > CRITERIA_MARGIN) {

            criteriaCounter = 0;
            return true;
        }
    }
    else criteriaCounter = 0;

    return false;
}

/*********************************************************************/

void Tasks::writeToFlash(bool force) {

    if (!appendFlash) {

        appendFlash = true;
        file = SPIFFS.open("/FlightData.apg", "w");
    }
    else {
        file = SPIFFS.open("/FlightData.apg", "a");
    }

    if (glob.dataFramesFifo.size() > 20 || force) {

        while(glob.dataFramesFifo.size()) {
            DataFrame tempData = glob.dataFramesFifo.front();
            glob.dataFramesFifo.pop();
            file.write((uint8_t*) &tempData, sizeof(tempData));
        }
    }

    file.close();
}

/*********************************************************************/

void Tasks::clearMem() {

    for (uint8_t i = 0; i < FLIGHTS_IN_MEM; i++) {

        glob.memory.lastFlightIndex = 0;
        glob.memory.lastFlightNum = 0;
        glob.memory.flight[i].num = 0;
        glob.memory.flight[i].apogee = 0;
        glob.memory.flight[i].maxSpeed = 0;
    }

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
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