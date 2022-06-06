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

    tasks.writeToFlash();

    while(1) {

        tasks.measure();
        glob.dataFramesFifo.push(glob.dataFrame);
        if (tasks.isApogeeDetected()) break;
    }

}

/*********************************************************************/

void StateLoops::sep1Loop() {

}

/*********************************************************************/

void StateLoops::sep2Loop() {

}

/*********************************************************************/

void StateLoops::groundLoop() {

}