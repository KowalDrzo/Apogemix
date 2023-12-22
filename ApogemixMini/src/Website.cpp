#include "Website.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void Website::start() {

    WiFi.softAP(ssid.c_str(), password);

    Serial.println("server on");
    enabled = true;

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", generateHtml());
    });

    server.on("/api", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", glob.dataFrame.toString());
    });

    server.on("/wifioff", HTTP_GET, [this](AsyncWebServerRequest *request) {

        stop();
    });

    server.on("/FlightData.apg", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(SPIFFS, "/FlightData.apg", String());
    });

    server.on("/e_recovery", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateSettingsPage(RECOVERY_SETTINGS));
    });

    server.on("/recovery_test", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", generateRecoveryTest());
    });

    server.on("/recovery_test1", HTTP_POST, [this](AsyncWebServerRequest *request) {

        glob.forceRecoveryTest = 1;
        request->send(200, "text/html", "OK");
    });

    server.on("/recovery_test2", HTTP_POST, [this](AsyncWebServerRequest *request) {

        glob.forceRecoveryTest = 2;
        request->send(200, "text/html", "OK");
    });

    server.begin();
}

#pragma GCC diagnostic pop

/*********************************************************************/

void Website::stop() {

    Serial.println("server off");
    WiFi.softAPdisconnect(true);
    enabled = false;
}

/*********************************************************************/

void Website::handleArgs(AsyncWebServerRequest *request) {

    uint8_t paramNb = request->params();

    if (paramNb) {

        for (uint8_t i = 0; i < paramNb; i++) {

            AsyncWebParameter* p = request->getParam(i);

            // Recovery params:
            if (p->name() == "setSecAlt") {

                uint16_t val = p->value().toInt();
                if (val >= 50 && val <= 5000) glob.memory.secondSeparAltitude = val;
            }
        }
        EEPROM.put(0, glob.memory);
        EEPROM.commit();
    }
}
