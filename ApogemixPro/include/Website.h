#ifndef WEBSITE_H
#define WEBSITE_H

#include "ESPAsyncWebServer.h"

#include "Tasks.h"

class Website {

    String ssid = String("Apogemix_pro_") + String(APOGEMIX_ID);
    const char* password = "ApogemixPro";
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