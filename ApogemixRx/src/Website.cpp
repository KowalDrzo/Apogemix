#include "Website.h"

namespace {

String makeApogemixSsid() {
    static const char* months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    char monthName[4] = {};
    int day = 0;
    int year = 0;
    sscanf(__DATE__, "%3s %d %d", monthName, &day, &year);

    uint8_t month = 0;
    for (uint8_t i = 0; i < 12; ++i) {
        if (strcmp(monthName, months[i]) == 0) {
            month = i + 1;
            break;
        }
    }

    const uint32_t macSuffix = static_cast<uint32_t>(ESP.getEfuseMac() & 0xFFFFFF);
    char suffix[16];
    snprintf(suffix, sizeof(suffix), "%06lX_%02d%02d%02d",
             static_cast<unsigned long>(macSuffix), year % 100, month, day);
    return String("APOGEMIX_RX_") + suffix;
}

}

void Website::start() {

    ssid = makeApogemixSsid();

    WiFi.mode(WIFI_AP);
    WiFi.setTxPower(WIFI_POWER_13dBm);

    WiFi.softAP(ssid.c_str(), password);
    MDNS.begin("apogemix");
    enabled = true;

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(SPIFFS, "/index.html", String());
    });

    server.on("/wifioff", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", String("<meta http-equiv=\"refresh\" content=\"0; URL=/\" />"));
        stop();
    });

    ws.onEvent([](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

        if (type == WS_EVT_DATA) {
            glob.txString = String((char*)data);
            glob.txRequest = true;
        }
    });

    server.addHandler(&ws);
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
                if (val >= 100 && val <= 500) glob.memory.frequencyMHz = val;
                saveAndSetNewFrequency();
            }
        }
    }
}
