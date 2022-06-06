#include "Tasks.h"

Tasks tasks;

void Tasks::measure() {

    glob.dataFrame.pressure = bmp.readPressure();
    glob.dataFrame.altitude = bmp.readAltitude(glob.initialPressure);
    
    glob.dataFrame.continuity1 = !digitalRead(CONT1_PIN);
    glob.dataFrame.continuity2 = !digitalRead(CONT2_PIN);

    glob.dataFrame.time = millis();

    Serial.println(glob.dataFrame.altitude);
}

/*********************************************************************/

bool Tasks::isLaunchDetected() {

    if (glob.dataFrame.altitude > 10) {

        criteriaCounter++;
        if (criteriaCounter > 2) {

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
        if (criteriaCounter > 2) {

            criteriaCounter = 0;
            return true;
        }
    }
    else criteriaCounter = 0;

    return false;
}

/*********************************************************************/

bool Tasks::isSecondChuteTime() {

    // TODO

    return false;
}

bool Tasks::isOnGround() {

    // TODO

    return false;
}

/*********************************************************************/

void Tasks::writeToFlash() {


    // TODO
}