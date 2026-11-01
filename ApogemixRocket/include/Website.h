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

    String ssid;
    const char* password = "apogemix";

    AsyncWebServer server = AsyncWebServer(80);
    bool enabled = false;
    bool routesRegistered = false;
    volatile bool rebootRequested = false;
    volatile uint32_t rebootRequestedAt = 0;

    String generateHtml();
    String generateSettingsPage(uint8_t settingsType);
    void handleArgs(AsyncWebServerRequest *request);
    String generateRecoveryTest();

public:

    bool isEnabled() { return enabled; }
    bool isRebootRequested() { return rebootRequested; }
    bool shouldReboot() {
        return rebootRequested && millis() - rebootRequestedAt >= 250;
    }
    void requestReboot() {
        rebootRequestedAt = millis();
        rebootRequested = true;
    }
    void start();
    void stop();
};

#endif
