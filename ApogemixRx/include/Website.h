#ifndef WEBSITE_H
#define WEBSITE_H

#include "ESPAsyncWebServer.h"
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include "SPIFFS.h"

#include "PinDefinitions.h"
#include "Functions.h"

class Website {

    String ssid = String("Apogemix_rx") + String(APOGEMIX_ID);
    const char* password = "apogemix";
    AsyncWebServer server = AsyncWebServer(80);
    AsyncWebSocket ws = AsyncWebSocket("/ws");
    bool enabled = false;
    void handleArgs(AsyncWebServerRequest *request);

    // For POST requests:
    String postDevice;
    String postCommand;

public:

    bool isEnabled() { return enabled; }
    void start();
    void stop();
    void sendWsString(String wsString) { ws.textAll(wsString); }
};

#endif
