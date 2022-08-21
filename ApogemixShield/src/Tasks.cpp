#include "Tasks.h"

Tasks tasks;

#define CRITERIA_MARGIN 5

void Tasks::continuityTest() {

    glob.dataFrame.continuity1 = !digitalRead(CONT1_PIN);
    glob.dataFrame.continuity2 = !digitalRead(CONT2_PIN);
}

/*********************************************************************/

void Tasks::measure() {

    // Pressure and temperature:
    glob.dataFrame.pressure = bmp.readPressure();
    glob.dataFrame.temper = bmp.readTemperature() + TEMPERATURE_OFFSET;

    // Altitude:
    float oldAlt = glob.dataFrame.altitude;
    float newAlt = (glob.initialTemper+273.15)/0.0065*(1.0 - pow(glob.dataFrame.pressure/glob.initialPressure, 0.1903));
    glob.dataFrame.altitude = ALPHA_H * oldAlt + (1-ALPHA_H) * newAlt;
    
    // Speed:
    float oldSpd = glob.dataFrame.speed;
    float newSpd = 1000.0*(glob.dataFrame.altitude - oldAlt)/(millis() - glob.dataFrame.time);
    glob.dataFrame.speed = ALPHA_V * oldSpd + (1-ALPHA_V) * newSpd;

    // Contunuity:
    continuityTest();

    // Time:
    glob.dataFrame.time = millis();

    // Maximas:
    if (glob.apogee < glob.dataFrame.altitude) glob.apogee = glob.dataFrame.altitude;
    if (glob.maxSpeed < glob.dataFrame.speed) glob.maxSpeed = glob.dataFrame.speed;

    // Debug:
    Serial.println(glob.dataFrame.toString());
}

/*********************************************************************/

void Tasks::buzzBeep(uint16_t time, uint8_t n) {

    for (; n > 0; n--) {

        digitalWrite(BUZZER_PIN, 1);
        delay(time);
        digitalWrite(BUZZER_PIN, 0);
        delay(time);
    }
}

/*********************************************************************/

void Tasks::buzz() {

    continuityTest();

    digitalWrite(BUZZER_PIN, 1);
    delay(500);
    digitalWrite(BUZZER_PIN, 0);
    delay(3000);

    if (glob.dataFrame.continuity1 && glob.dataFrame.continuity2)   buzzBeep(500, 3);
    else if (glob.dataFrame.continuity1)                            buzzBeep(500, 1);
    else if (glob.dataFrame.continuity2)                            buzzBeep(500, 2);
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

/*********************************************************************/

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void Tasks::writeToFlash(bool force) {

    if (!appendFlash) {

        appendFlash = true;
        file = LITTLEFS.open("/FlightData.apg", "w");
    }
    else {
        file = LITTLEFS.open("/FlightData.apg", "a");
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

void Tasks::readFlash() {

    LITTLEFS.begin(true);

    file = LITTLEFS.open("/FlightData.apg", "r");

    DataFrame dane;
    while (file.readBytes((char*) &dane, sizeof(dane))) {
        Serial.println(dane.toString());
    }

    file.close();
}

#pragma GCC diagnostic pop

/*********************************************************************/

void Tasks::clearMem() {

    LITTLEFS.begin(true);
    file = LITTLEFS.open("/FlightData.apg", "w");
    file.close();

    memset(&glob.memory, 0, sizeof(glob.memory));
    glob.memory.wifiActiveTime_min = 3;

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