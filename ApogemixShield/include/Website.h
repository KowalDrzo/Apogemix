#ifndef WEBSITE_H
#define WEBSITE_H

#include "ESPAsyncWebServer.h"

#include "Tasks.h"

class Website {

    const char* ssid = "Apogemix_shield_1";
    const char* password = "ApogemixShield";
    AsyncWebServer server = AsyncWebServer(80);
    bool enabled = false;

public:

    bool isEnabled() { return enabled; }
    void start();
    void stop();
    String generateHtml();
    void handleArgs(AsyncWebServerRequest *request);
};

#endif