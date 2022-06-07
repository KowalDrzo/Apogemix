#include "Website.h"

void Website::start() {

    WiFi.softAP(ssid, password);
    MDNS.begin("apogemix");

    Serial.println("server on");
    enabled = true;

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {

        request->send(200, "text/plain", String(random(1000)));
    });
 
    server.begin();
}

/*********************************************************************/

void Website::stop() {

    Serial.println("server off");
    WiFi.mode(WIFI_OFF);
    enabled = false;
}