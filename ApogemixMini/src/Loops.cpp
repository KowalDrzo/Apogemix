#include "Loops.h"

StateLoops loops;

void StateLoops::dataLoop(bool enableFlashWrite) {

    tasks.measure();
    glob.dataFramesFifo.push(glob.dataFrame);
    if (enableFlashWrite) tasks.writeToFlash();
}

/*********************************************************************/

void StateLoops::ignitionLoop(bool apogee) {
    
    ignitionTimer.start(2000);

    if (apogee) digitalWrite(SEPAR1_PIN, 1);
    else digitalWrite(SEPAR2_PIN, 1);

    while (!ignitionTimer.check()) {

        if (pressMeasureTimer.check()) dataLoop(1);
    }

    digitalWrite(SEPAR1_PIN, 0);
    digitalWrite(SEPAR2_PIN, 0);
}

/*********************************************************************/

void StateLoops::railLoop() {

    pressMeasureTimer.start(200);
    Website website;

    while (1) {

        if (pressMeasureTimer.check()) {
            
            dataLoop(0);
            if (tasks.isLaunchDetected()) break;

            if (!digitalRead(SWITCH_PIN) && !website.isEnabled()) {

                website.start();
            }
        }
    }
}

/*********************************************************************/

void StateLoops::flightLoop() {

    pressMeasureTimer.start(200);

    while(1) {

        if (pressMeasureTimer.check()) {

            dataLoop(1);
            if (tasks.isApogeeDetected()) {

                ignitionLoop(1);
                break;
            }
        }
    }
}

/*********************************************************************/

void StateLoops::sep1Loop() {

    pressMeasureTimer.start(500);

    while(1) {

        if (pressMeasureTimer.check()) {

            dataLoop(1);
            if (tasks.isSecondChuteTime()) {

                ignitionLoop(0);
                break;
            }
        }
    }
}

/*********************************************************************/

void StateLoops::sep2Loop() {

    pressMeasureTimer.start(1000);

    while(1) {

        if (pressMeasureTimer.check()) {

            dataLoop(1);
            if (tasks.isOnGround()) break;
        }
    }
}

/*********************************************************************/

void StateLoops::groundLoop() {

    pressMeasureTimer.start(3000);

    while (1) {

        if (pressMeasureTimer.check()) {

            tasks.writeToFlash();
        }
    }
}