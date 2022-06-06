#ifndef WEBSITE_H
#define WEBSITE_H

#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

class Website {

    const char* ssid = "Apogemix_mini_1";
    const char* password = "ApogemixMini";
    AsyncWebServer server = AsyncWebServer(80);
    bool enabled = false;

public:

    bool isEnabled() { return enabled; }
    void start();
    void stop();
};

#endif