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

void Tasks::writeToFlash() {

    if (!appendFlash) {

        appendFlash = true;
        file = SPIFFS.open("/FlightData.apg", "w");
    }
    else {
        file = SPIFFS.open("/FlightData.apg", "a");
    }

    if (glob.dataFramesFifo.size() > 20) {

        while(glob.dataFramesFifo.size()) {
            DataFrame tempData = glob.dataFramesFifo.front();
            glob.dataFramesFifo.pop();
            file.write((uint8_t*) &tempData, sizeof(tempData));
        }
    }

    file.close();
}