#include "Loops.h"

StateLoops loops;

void StateLoops::railLoop() {

    pressMeasureTimer.start(200);

    while (1) {

        if (pressMeasureTimer.check()) {
            
            tasks.measure();
            glob.dataFramesFifo.push(glob.dataFrame);
            if (tasks.isLaunchDetected()) break;
        }
    }
}

/*********************************************************************/

void StateLoops::flightLoop() {

    pressMeasureTimer.start(200);

    while(1) {

        if (pressMeasureTimer.check()) {

            tasks.measure();
            glob.dataFramesFifo.push(glob.dataFrame);
            tasks.writeToFlash();

            if (tasks.isApogeeDetected()) break;
        }
    }
}

/*********************************************************************/

void StateLoops::sep1Loop() {

    pressMeasureTimer.start(500);

    while(1) {

        if (pressMeasureTimer.check()) {

            tasks.measure();
            glob.dataFramesFifo.push(glob.dataFrame);
            tasks.writeToFlash();
            
            if (tasks.isSecondChuteTime()) break;
        }
    }
}

/*********************************************************************/

void StateLoops::sep2Loop() {

    pressMeasureTimer.start(1000);

    while(1) {

        if (pressMeasureTimer.check()) {

            tasks.measure();
            glob.dataFramesFifo.push(glob.dataFrame);
            tasks.writeToFlash();
            
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