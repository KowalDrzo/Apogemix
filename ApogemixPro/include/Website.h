#ifndef WEBSITE_H
#define WEBSITE_H

#include "ESPAsyncWebServer.h"
#include <ESPmDNS.h>

#include "Tasks.h"

class Website {

    enum SettingsType {
        RECOVERY_SETTINGS = 0,
        TELEMETRY_SETTINGS,
        SERVOS_SETTINGS
    };

    String ssid = String("Apogemix_pro_") + String(APOGEMIX_ID);
    const char* password = "ApogemixPro";
    AsyncWebServer server = AsyncWebServer(80);
    bool enabled = false;

    String generateHtml();
    String generateSettingsPage(uint8_t settingsType);
    void handleArgs(AsyncWebServerRequest *request);

public:

    bool isEnabled() { return enabled; }
    void start();
    void stop();
};

#endif