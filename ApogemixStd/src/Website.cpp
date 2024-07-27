#include "Website.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void Website::start() {

    WiFi.softAP(ssid.c_str(), password);
    MDNS.begin("apogemix");

    Serial.println("server on");
    enabled = true;

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", generateHtml());
    });

    server.on("/api", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", glob.dataFrame.toString());
    });

    server.on("/wifioff", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", String("<meta http-equiv=\"refresh\" content=\"0; URL=/\" />"));
        stop();
    });

    server.on("/FlightData.apg", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(LittleFS, "/FlightData.apg", String());
    });

    server.on("/e_recovery", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateSettingsPage(RECOVERY_SETTINGS));
    });

    server.on("/e_telemetry", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateSettingsPage(TELEMETRY_SETTINGS));
    });

    server.on("/e_servos", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateSettingsPage(SERVOS_SETTINGS));
    });

    server.on("/recovery_test", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", generateRecoveryTest());
    });

    server.on("/recovery_test1", HTTP_POST, [this](AsyncWebServerRequest *request) {

        digitalWrite(BUZZER_PIN, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        digitalWrite(BUZZER_PIN, 0);
        digitalWrite(SEPAR1_PIN, 1);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        digitalWrite(SEPAR1_PIN, 0);
        request->send(200, "text/html", "OK");
    });

    server.on("/recovery_test2", HTTP_POST, [this](AsyncWebServerRequest *request) {

        digitalWrite(BUZZER_PIN, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        digitalWrite(BUZZER_PIN, 0);
        digitalWrite(SEPAR2_PIN, 1);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        digitalWrite(SEPAR2_PIN, 0);
        request->send(200, "text/html", "OK");
    });

    server.on("/set_servos", HTTP_POST,
        [](AsyncWebServerRequest * request){},
        NULL,
        [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

            char buffer[100];
            strncpy(buffer, (char*) data, len);
            uint8_t angle1, angle2;
            sscanf(buffer, "%d;%d", &angle1, &angle2);

            request->send(200);
        }
    );

    server.begin();
}

#pragma GCC diagnostic pop

/*********************************************************************/

void Website::stop() {

    Serial.println("server off");
    //WiFi.softAPdisconnect(true);
    enabled = false;
}

/*********************************************************************/

void Website::handleArgs(AsyncWebServerRequest *request) {

    uint8_t paramNb = request->params();

    if (paramNb) {

        for (uint8_t i = 0; i < paramNb; i++) {

            AsyncWebParameter* p = request->getParam(i);

            // Recovery params:
            if (p->name() == "dataSave") {

                if      (p->value() == "1") glob.memory.isCsvFile = 1;
                else if (p->value() == "0") glob.memory.isCsvFile = 0;
            }

            else if (p->name() == "sep1Mode") {

                if      (p->value() == "1") glob.memory.isSep1BeforeApog = 1;
                else if (p->value() == "0") glob.memory.isSep1BeforeApog = 0;
            }

            else if (p->name() == "sep2Mode") {

                if      (p->value() == "1") glob.memory.isSep2Staging = 1;
                else if (p->value() == "0") glob.memory.isSep2Staging = 0;
            }

            else if (p->name() == "setStaging") {

                uint16_t val = p->value().toInt();
                if (val >= 2000 && val <= 30000) glob.memory.stagingDelay = val;
            }

            else if (p->name() == "setSecAlt") {

                uint16_t val = p->value().toInt();
                if (val >= 50 && val <= 5000) glob.memory.secondSeparAltitude = val;
            }

            // Telemetry params:
            else if (p->name() == "setLoraFreq") {

                uint16_t val = p->value().toInt();
                if (val >= 100 && val <= 999) glob.memory.loraFreqMHz = val;
            }

            else if (p->name() == "setLoRaDelay") {

                uint16_t val = p->value().toInt();
                if (val >= 500 && val <= 5000) glob.memory.loraDelay_ms = val;
            }

            else if (p->name() == "setCallSign" && p->value().length() >= 2) {

                String newCall = p->value();
                strncpy(glob.memory.callsign, newCall.c_str(), CALLSIGN_LEN - 1);
                glob.memory.callsign[CALLSIGN_LEN - 1] = 0;
            }
        }
        EEPROM.put(0, glob.memory);
        EEPROM.commit();
    }
}
