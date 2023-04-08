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
    glob.dataFrame.temper = bmp.readTemperature() * TEMPERATURE_FIX_A + TEMPERATURE_FIX_B;

    // Time:
    uint32_t newTime = millis();
    uint32_t timeDiff = newTime - glob.dataFrame.time;
    if (!timeDiff) timeDiff++;
    glob.dataFrame.time = newTime;

    // Altitude:
    float oldAlt = glob.dataFrame.altitude;
    float newAlt = (glob.initialTemper+273.15)/0.0065*(1.0 - pow(glob.dataFrame.pressure/glob.initialPressure, 0.1903));
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
        vTaskDelay(activeTime / portTICK_PERIOD_MS);
        digitalWrite(BUZZER_PIN, 0);
        vTaskDelay(sleepTime / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/

void Tasks::buzz() {

    continuityTest();

    buzzBeep(30, 150, 2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    if (glob.dataFrame.continuity1 && glob.dataFrame.continuity2)   buzzBeep(500, 500, 3);
    else if (glob.dataFrame.continuity1)                            buzzBeep(500, 500, 1);
    else if (glob.dataFrame.continuity2)                            buzzBeep(500, 500, 2);
}

/*********************************************************************/

bool Tasks::isLaunchDetected() {

    if (glob.dataFrame.altitude > 20) {

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

    if (glob.dataFrame.speed < -1 || (glob.memory.isSep1BeforeApog && glob.dataFrame.speed < 10)) {

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

void Tasks::flashTask() {

    File file;
    bool appendFlash = false;

    while (glob.dataFrame.rocketState < FLIGHT) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    while (1) {

        if (uxQueueMessagesWaiting(glob.dataFramesFifo) > 20 || glob.dataFrame.rocketState == GROUND) {

            if (!appendFlash) {
                appendFlash = true;
                file = LITTLEFS.open("/FlightData.apg", "w");
            }
            else {
                file = LITTLEFS.open("/FlightData.apg", "a");
            }

            while (uxQueueMessagesWaiting(glob.dataFramesFifo)) {

                DataFrame tempData;
                xQueueReceive(glob.dataFramesFifo, &tempData, portMAX_DELAY);

                if (glob.memory.isCsvFile) {
                    char tempDataAscii[120];
                    strcpy(tempDataAscii, tempData.toString().c_str());
                    strcat(tempDataAscii, "\n");
                    file.write((uint8_t*) tempDataAscii, strlen(tempDataAscii));
                }
                else file.write((uint8_t*) &tempData, sizeof(tempData));
            }

            file.close();
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/

void Tasks::readFlash() {

    LITTLEFS.begin(true);

    File file = LITTLEFS.open("/FlightData.apg", "r");

    if (glob.memory.isCsvFile) {
        while (file.available()) Serial.print(file.readString());
    }
    else {
        DataFrame dane;
        while (file.readBytes((char*) &dane, sizeof(dane))) {
            Serial.println(dane.toString());
        }
    }

    file.close();
}

#pragma GCC diagnostic pop

/*********************************************************************/

void Tasks::clearMem() {

    LITTLEFS.begin(true);
    File file = LITTLEFS.open("/FlightData.apg", "w");

    memset(&glob.memory, 0, sizeof(glob.memory));

    glob.memory.secondSeparAltitude = 100;
    glob.memory.loraDelay_ms = 2000;
    glob.memory.loraFreqMHz = 433;
    strcpy(glob.memory.callsign, "APOGEMIX");

    EEPROM.put(0, glob.memory);
    EEPROM.commit();

    buzzBeep(30, 30, 10);
    while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
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

/*********************************************************************/

void Tasks::servosInit() {

    servos[0].attach(SERVO_1_PIN);
    servos[1].attach(SERVO_2_PIN);

    servos[0].write(glob.memory.servo1Initial);
    servos[1].write(glob.memory.servo2Initial);
}

/*********************************************************************/

void Tasks::servosSet(bool isApogee) {

    if (isApogee) {
        servos[0].write(glob.memory.servo1Apog);
        servos[1].write(glob.memory.servo2Apog);
    }
    else {
        servos[0].write(glob.memory.servo1dd);
        servos[1].write(glob.memory.servo2dd);
    }
}