#include "Website.h"

void Website::start() {

    //WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    //WiFi.hostname("Apogemix.mini");

    Serial.println("server on");
    enabled = true;

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {

        request->send(200, "text/plain", String(random(1000)));
    });
 
    server.begin();
}

/*********************************************************************/

void Website::stop() {

    WiFi.mode(WIFI_OFF);
}