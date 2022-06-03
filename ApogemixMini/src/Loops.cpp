#include "Loops.h"

StateLoops loops;

void StateLoops::railLoop() {

    pressMeasureTimer.start(200);

    while (1) {

        if (pressMeasureTimer.check()) {
            tasks.measure();
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