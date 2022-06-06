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

        launchDetectionCriteria++;
        if (launchDetectionCriteria > 2) return true;
    }
    else launchDetectionCriteria = 0;

    return false;
}

/*********************************************************************/

bool isApogeeDetected() {

    // TODO

    return false;
}

/*********************************************************************/

void Tasks::writeToFlash() {


    // TODO
}