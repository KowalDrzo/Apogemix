#include "Loops.h"

StateLoops loops;

void StateLoops::dataLoop(bool enableFlashWrite) {

    tasks.measure();
    DataFrame trashBin;
    if (uxQueueMessagesWaiting(glob.dataFramesFifo) == FRAMES_IN_Q) {
        xQueueReceive(glob.dataFramesFifo, &trashBin, portMAX_DELAY);
    }
    xQueueSend(glob.dataFramesFifo, &glob.dataFrame, portMAX_DELAY);
}

/*********************************************************************/

void StateLoops::ignitionLoop(bool apogee) {

    tasks.servosSet(apogee);
    if (apogee) digitalWrite(SEPAR1_PIN, 1);
    // Depending on the P2 mode - TODO!!!
    //else digitalWrite(SEPAR2_PIN, 1);

    waitAndLogData(FIRE_TIME);

    digitalWrite(SEPAR1_PIN, 0);

    // Depending on the P2 mode - TODO!!!
    // digitalWrite(SEPAR2_PIN, 0);
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
    tasks.servosInit();

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
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);

        // DEBUG:
        if (Serial.available()) {
            rxDebugString = Serial.readString();
            if (strstr(rxDebugString.c_str(), "FORCE NEXT STATE")) break;
        }
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

        // DEBUG:
        if (Serial.available()) {
            rxDebugString = Serial.readString();
            if (strstr(rxDebugString.c_str(), "FORCE NEXT STATE")) break;
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

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    // Depending on the P2 mode - TODO!!!
    digitalWrite(SEPAR2_PIN, 0);
}

/*********************************************************************/

void StateLoops::gpsLoop() {

    while(1) {

        if (Serial1.available()) {

            char c = Serial1.read();
            if(tasks.gps.encode(c)) {

                glob.dataFrame.gpsLat = tasks.gps.location.lat();
                glob.dataFrame.gpsLng = tasks.gps.location.lng();
                glob.dataFrame.gpsAlt = tasks.gps.altitude.meters();
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

    bool gpsFound = false;

    //ledcSetup(0, 2000, 8);
    //ledcAttachPin(BUZZER_PIN, 0);

    tasks.buzz();

    loraTimer.start(glob.memory.loraDelay_ms);

    hspi.begin(5, 19, 27, 18);
    LoRa.setSPI(hspi);
    LoRa.setPins(18, 23, 26);
    LoRa.setSignalBandwidth(125E3);
    LoRa.begin(glob.memory.loraFreqMHz * 1E6);
    LoRa.setTimeout(100);

    while (1) {

        // Tx:
        if (loraTimer.check()) {

            loraString = String(glob.memory.callsign) + String(";") + glob.dataFrame.toString();
            LoRa.beginPacket();
            LoRa.println(loraString);
            LoRa.endPacket();

            // GPS fix buzz:
            if (glob.dataFrame.gpsLat != 0 && !gpsFound) {

                gpsFound = true;
                tasks.buzzBeep(30, 150, 3);
            }
        }

        // Rx:
        if (LoRa.parsePacket()) {
            while (LoRa.available()) {

                loraRxCallback(LoRa.readString());
            }
        }

        // GROUND state:
        if (glob.dataFrame.rocketState == GROUND) {

            while (1) {

                digitalWrite(BUZZER_PIN, 0);
                vTaskDelay(9000 / portTICK_PERIOD_MS);

                loraString = String(glob.memory.callsign) + String(";") + glob.dataFrame.toString();
                LoRa.beginPacket();
                LoRa.println(loraString);
                LoRa.endPacket();

                digitalWrite(BUZZER_PIN, 1);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/

void StateLoops::loraRxCallback(String rxFrame) {

    if (strstr(rxFrame.c_str(), glob.memory.callsign)) {

        if (strstr(rxFrame.c_str(), "TEST1") && (glob.dataFrame.rocketState <= FLIGHT)) {

            digitalWrite(BUZZER_PIN, 1);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            digitalWrite(BUZZER_PIN, 0);
            digitalWrite(SEPAR1_PIN, 1);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            digitalWrite(SEPAR1_PIN, 0);
        }

        else if (strstr(rxFrame.c_str(), "TEST2") && glob.dataFrame.rocketState <= FIRST_SEPAR) {

            digitalWrite(BUZZER_PIN, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            digitalWrite(BUZZER_PIN, 0);
            digitalWrite(SEPAR2_PIN, !digitalRead(SEPAR2_PIN));

            // Depending on the P2 mode - TODO!!!
            /*digitalWrite(BUZZER_PIN, 1);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            digitalWrite(BUZZER_PIN, 0);
            digitalWrite(SEPAR2_PIN, 1);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            digitalWrite(SEPAR2_PIN, 0);*/
        }

        else if (strstr(rxFrame.c_str(), "MOS_ON")) {
            // TODO Additional mosfet on.
        }

        else if (strstr(rxFrame.c_str(), "MOS_OFF")) {
            // TODO Additional mosfet off.
        }

        else if (strstr(rxFrame.c_str(), "RECAALIBRATE") && (glob.dataFrame.rocketState < FLIGHT)) {

            tasks.recalibrate();
        }
    }
}