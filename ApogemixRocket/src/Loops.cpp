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
            // WiFi itp:
            if (!website.isEnabled()
            ) {

                website.start();
            }
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/

#define STAGING_TIME_OFFSET_MS 1000

void StateLoops::flightLoop() {

    pressMeasureTimer.start(RAIL_FLIGHT_LOOP_TIME);
    stagingTimer.start(glob.memory.stagingDelay - STAGING_TIME_OFFSET_MS);

    while(1) {

        if (pressMeasureTimer.check()) {

            dataLoop(1);
            if (glob.dataFrame.speed > 30) {
                glob.isSep1BeforeApog = true;
            }

            if (tasks.isApogeeDetected()) {

                ignitionLoop(1);
                break;
            }
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);

        // Staging:
        if (glob.memory.isSep2Staging && stagingTimer.check()) {

            ignitionLoop(0);
            glob.memory.isSep2Staging = false;
        }

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
            if (tasks.isSecondChuteTime() && glob.memory.isSep2Staging == false) {

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
    String loraString;

    bool gpsFound = false;
    bool loraActive = false;

    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, LORA_CS_PIN);
    LoRa.setPins(LORA_CS_PIN, LORA_RES_PIN, LORA_D0_PIN);
    LoRa.setSignalBandwidth(125E3);
    loraActive = LoRa.begin(glob.memory.loraFreqMHz * 1E6);
    LoRa.setTimeout(100);

    tasks.buzz(loraActive ? 3 : 2);

    loraTimer.start(glob.memory.loraDelay_ms);

    while (loraActive) {

        // Tx:
        if (loraTimer.check()) {

            loraString = String(glob.memory.callsign) + String(";") + glob.dataFrame.toString();
            LoRa.beginPacket();
            LoRa.println(loraString);
            LoRa.endPacket();

            // GPS fix buzz:
            if (glob.dataFrame.gpsLat != 0 && !gpsFound) {

                gpsFound = true;
                tasks.buzzBeep(30, 150, 4);
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

                vTaskDelay(9000 / portTICK_PERIOD_MS);

                loraString = String(glob.memory.callsign) + String(";") + glob.dataFrame.toString();
                LoRa.beginPacket();
                LoRa.println(loraString);
                LoRa.endPacket();
            }
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

/*********************************************************************/

void StateLoops::loraRxCallback(String rxFrame) {

    if (strstr(rxFrame.c_str(), glob.memory.callsign)) {

        if (strstr(rxFrame.c_str(), "TEST1") && (glob.dataFrame.rocketState <= FLIGHT)) {

            digitalWrite(BUZZER_PIN, 1);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            digitalWrite(BUZZER_PIN, 0);
            digitalWrite(SEPAR1_PIN, 1);
        }

        else if (strstr(rxFrame.c_str(), "TEST2") && glob.dataFrame.rocketState <= FIRST_SEPAR) {

            digitalWrite(BUZZER_PIN, 1);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            digitalWrite(BUZZER_PIN, 0);
            digitalWrite(SEPAR2_PIN, 1);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            digitalWrite(SEPAR2_PIN, 0);
            digitalWrite(SEPAR1_PIN, 0);
        }

        else if (strstr(rxFrame.c_str(), "RECALIBRATE") && (glob.dataFrame.rocketState < FLIGHT)) {

            digitalWrite(SEPAR1_PIN, 0);
        }
    }
}