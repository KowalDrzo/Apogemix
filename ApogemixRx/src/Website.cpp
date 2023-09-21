#include "Website.h"

void Website::start() {

    WiFi.softAP(ssid.c_str(), password);
    MDNS.begin("apogemix");
    enabled = true;

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateHtml());
    });

    server.on("/devices", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", getDevicesFromMap());
    });

    server.on("/frame", HTTP_GET, [this](AsyncWebServerRequest *request) {

        String deviceName = getDeviceFromArgs(request);
        request->send(200, "text/html", getFrameFromMap(deviceName));
    });

    server.on("/wifioff", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", String("<meta http-equiv=\"refresh\" content=\"0; URL=/\" />"));
        stop();
    });

    server.begin();

    for(uint8_t i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(LED_PIN, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/

void Website::stop() {

    WiFi.softAPdisconnect(true);
    enabled = false;
}

/*********************************************************************/

void Website::handleArgs(AsyncWebServerRequest *request) {

    uint8_t paramNb = request->params();

    if (paramNb) {

        for (uint8_t i = 0; i < paramNb; i++) {

            AsyncWebParameter* p = request->getParam(i);

            if (p->name() == "freqmhz") {

                uint16_t val = p->value().toInt();
                if (val >= 100 && val <= 500) glob.frequencyMHz = val;
            }
        }
        saveAndSetNewFrequency();
    }
}

String Website::getDeviceFromArgs(AsyncWebServerRequest *request) {

    uint8_t paramNb = request->params();

    if (paramNb) {

        for (uint8_t i = 0; i < paramNb; i++) {

            AsyncWebParameter* p = request->getParam(i);
            if (p->name() == "device") {
                return p->value();
            }
        }
    }

    return String("");
}