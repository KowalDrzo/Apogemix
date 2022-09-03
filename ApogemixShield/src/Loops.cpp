#include "Loops.h"

StateLoops loops;
Tasks tasks;

/*********************************************************************/

void StateLoops::gpsLoop() {

    while(1) {

        if (Serial1.available()) {

            char c = Serial1.read();
            if(tasks.gps.encode(c)) {

                dataFrame.gpsLat = tasks.gps.location.lat();
                dataFrame.gpsLng = tasks.gps.location.lng();
                dataFrame.gpsAlt = tasks.gps.altitude.meters();
                dataFrame.gpsSec = tasks.gps.time.second();
            }
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/

void StateLoops::loraLoop() {

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    SPIClass hspi(HSPI);
    String loraString;

    hspi.begin(5, 19, 27, 18);
    LoRa.setSPI(hspi);
    LoRa.setPins(18, 23, 26);
    LoRa.setSignalBandwidth(125E3);
    LoRa.begin(439E6);
    LoRa.setTimeout(100);

    while (1) {

        loraString = String("BREAKER") + String(";") + dataFrame.toString();
        LoRa.beginPacket();
        LoRa.println(loraString);
        LoRa.endPacket();
        Serial.println(loraString);

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}