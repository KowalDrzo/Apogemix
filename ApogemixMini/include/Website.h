#ifndef WEBSITE_H
#define WEBSITE_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

#include "Tasks.h"

class Website {

    enum SettingsType {
        RECOVERY_SETTINGS = 0,
        TELEMETRY_SETTINGS,
        SERVOS_SETTINGS
    };

    String ssid = String("Apogemix_mini_") + String(APOGEMIX_ID);
    const char* password = "ApogemixMini";
    AsyncWebServer server = AsyncWebServer(80);
    bool enabled = false;

    String generateHtml();
    String generateSettingsPage(uint8_t settingsType);
    void handleArgs(AsyncWebServerRequest *request);
    String generateRecoveryTest();

public:

    bool isEnabled() { return enabled; }
    void start();
    void stop();
};

#endif