#include "Loops.h"

StateLoops loops;

void StateLoops::dataLoop(bool enableFlashWrite) {

    tasks.measure();
    glob.dataFramesFifo.push(glob.dataFrame);
    if (enableFlashWrite) tasks.writeToFlash();
}

/*********************************************************************/

void StateLoops::ignitionLoop(bool apogee) {

    if (apogee) {
        
        waitAndLogData(glob.memory.firstSeparDelay_ms);
        digitalWrite(SEPAR1_PIN, 1);
    }
    else digitalWrite(SEPAR2_PIN, 1);

    waitAndLogData(FIRE_TIME);

    digitalWrite(SEPAR1_PIN, 0);
    digitalWrite(SEPAR2_PIN, 0);
}

/*********************************************************************/

void StateLoops::waitAndLogData(uint32_t time_ms) {

    waitingTimer.start(time_ms);

    while (!waitingTimer.check()) {

        if (pressMeasureTimer.check()) dataLoop(1);

        vTaskDelay(1);
    }
}

/*********************************************************************/
/*********************************************************************/

void StateLoops::railLoop() {

    pressMeasureTimer.start(RAIL_FLIGHT_LOOP_TIME);
    Website website;

    while (1) {

        if (pressMeasureTimer.check()) {
            
            dataLoop(0);
            if (tasks.isLaunchDetected()) {

                if (website.isEnabled()) website.stop();
                break;
            }

            // WiFi itp:
            if (!digitalRead(SWITCH_PIN) && !website.isEnabled()) {

                website.start();
                wifiTimer.start(glob.memory.wifiActiveTime_min * 60000);
            }
            if (website.isEnabled() && wifiTimer.check()) {

                website.stop();
            }
        }
        vTaskDelay(1);
    }
}

/*********************************************************************/

void StateLoops::flightLoop() {

    pressMeasureTimer.start(RAIL_FLIGHT_LOOP_TIME);

    while(1) {

        if (pressMeasureTimer.check()) {

            dataLoop(1);
            if (tasks.isApogeeDetected()) {

                ignitionLoop(1);
                break;
            }
        }
        vTaskDelay(1);
    }
}

/*********************************************************************/

void StateLoops::sep1Loop() {

    pressMeasureTimer.start(FIRST_SEPAR_LOOP_TIME);

    while(1) {

        if (pressMeasureTimer.check()) {

            dataLoop(1);
            if (tasks.isSecondChuteTime()) {

                ignitionLoop(0);
                break;
            }
        }
        vTaskDelay(1);
    }
}

/*********************************************************************/

void StateLoops::sep2Loop() {

    pressMeasureTimer.start(SECND_SEPAR_LOOP_TIME);

    while(1) {

        if (pressMeasureTimer.check()) {

            dataLoop(1);
            if (tasks.isOnGround()) break;
        }
        vTaskDelay(1);
    }
}

/*********************************************************************/

void StateLoops::groundLoop() {

    tasks.writeToFlash(1);
    vTaskDelay(1);
}