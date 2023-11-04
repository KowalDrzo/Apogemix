#include "Loops.h"

StateLoops loops;

void StateLoops::dataLoop(bool enableFlashWrite) {

    tasks.measure();
    glob.dataFramesFifo.push(glob.dataFrame);
    if (enableFlashWrite) tasks.writeToFlash();
}

/*********************************************************************/

void StateLoops::ignitionLoop(bool apogee) {

    if (apogee) digitalWrite(SEPAR1_PIN, 1);
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
                wifiTimer.start(WIFI_TIME_MS);
            }
            if (website.isEnabled() && wifiTimer.check()) {

                website.stop();
            }

            // Forced recovery tests:
            if (glob.forceRecoveryTest) {

                uint8_t recoveryPin = (glob.forceRecoveryTest == 1 ? SEPAR1_PIN : SEPAR2_PIN);
                glob.forceRecoveryTest = 0;

                digitalWrite(BUZZER_PIN, 1);
                delay(1000);
                digitalWrite(BUZZER_PIN, 0);
                digitalWrite(recoveryPin, 1);
                delay(2000);
                digitalWrite(recoveryPin, 0);
            }

            // DEBUG:
            if (Serial.available()) {
                String rxDebugString = Serial.readString();
                if (strstr(rxDebugString.c_str(), "FORCE NEXT STATE")) break;
            }
        }
    }
}

/*********************************************************************/

void StateLoops::flightLoop() {

    pressMeasureTimer.start(RAIL_FLIGHT_LOOP_TIME);
    tasks.buzzBeep(30, 1, 1);

    while(1) {

        if (pressMeasureTimer.check()) {

            dataLoop(1);
            if (tasks.isApogeeDetected()) {

                ignitionLoop(1);
                break;
            }

            // DEBUG:
            if (Serial.available()) {
                String rxDebugString = Serial.readString();
                if (strstr(rxDebugString.c_str(), "FORCE NEXT STATE")) break;
            }
        }
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
    }
}

/*********************************************************************/

void StateLoops::groundLoop() {

    tasks.writeToFlash(1);
}