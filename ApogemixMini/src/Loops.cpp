#include "Loops.h"

StateLoops loops;

void StateLoops::railLoop() {

    pressMeasureTimer.start(200);

    uint32_t testTim = millis();

    while (1) {

        if (pressMeasureTimer.check()) {
            tasks.measure();
            glob.dataFramesFifo.push(glob.dataFrame);
            Serial.println(glob.dataFramesFifo.size());

            Serial.println(millis() - testTim);
            testTim = millis();
        }
    }
}

/*********************************************************************/

void StateLoops::flightLoop() {

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