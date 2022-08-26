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

        vTaskDelay(1 / portTICK_PERIOD_MS);
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
        vTaskDelay(1 / portTICK_PERIOD_MS);
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
        vTaskDelay(1 / portTICK_PERIOD_MS);
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
        vTaskDelay(1 / portTICK_PERIOD_MS);
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
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/

void StateLoops::groundLoop() {

    tasks.writeToFlash(1);
    vTaskDelay(1 / portTICK_PERIOD_MS);
}

/*********************************************************************/

void StateLoops::gpsLoop() {

    bool gpsNotFixed = true;

    while(1) {

        if (Serial1.available()) {

            char c = Serial1.read();
            if(tasks.gps.encode(c)) {

                glob.dataFrame.gpsLat = tasks.gps.location.lat();
                glob.dataFrame.gpsLng = tasks.gps.location.lng();
                glob.dataFrame.gpsAlt = tasks.gps.altitude.meters();
            }
        }

        if (gpsNotFixed) {

            if(fabs(glob.dataFrame.gpsLat) > 0.001) {
                gpsNotFixed = false;
                glob.initialPressure = glob.dataFrame.pressure;
                glob.initialTemper = glob.dataFrame.temper;
            }
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/

void StateLoops::loraLoop() {

    Timer loraTimer;
    SPIClass hspi(HSPI);
    String loraString;

    ledcSetup(0, 2000, 8);
    ledcAttachPin(BUZZER_PIN, 0);

    tasks.buzz();

    loraTimer.start(glob.memory.loraDelay_ms);

    hspi.begin(5, 19, 27, 18);
    LoRa.setSPI(hspi);
    LoRa.setPins(18, 23, 26);
    LoRa.setSignalBandwidth(125E3);
    LoRa.begin(glob.memory.loraFreqMHz * 1E6);
    LoRa.setTimeout(100);

    while (1) {

        if (loraTimer.check()) {

            loraString = String(glob.memory.callsign) + String(";") + glob.dataFrame.toString();
            LoRa.beginPacket();
            LoRa.println(loraString);
            LoRa.endPacket();
        }

        if (glob.dataFrame.rocketState == GROUND) {

            while (1) {

                ledcWriteTone(0, 0); //digitalWrite(BUZZER_PIN, 0);
                vTaskDelay(9000 / portTICK_PERIOD_MS);

                loraString = String(glob.memory.callsign) + String(";") + glob.dataFrame.toString();
                LoRa.beginPacket();
                LoRa.println(loraString);
                LoRa.endPacket();

                ledcWriteTone(0, 2000); //digitalWrite(BUZZER_PIN, 1);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}