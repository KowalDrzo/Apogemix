#include "Website.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void Website::start() {

    WiFi.softAP(ssid, password);
    MDNS.begin("apogemix");

    Serial.println("server on");
    enabled = true;

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateHtml());
    });

    server.on("/wifioff", HTTP_GET, [this](AsyncWebServerRequest *request) {

        stop();
    });

    server.on("/FlightData.apg", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(SPIFFS, "/FlightData.apg", String());
    });

    server.begin();
}

#pragma GCC diagnostic pop

/*********************************************************************/

void Website::stop() {

    Serial.println("server off");
    WiFi.softAPdisconnect(true);
    enabled = false;
}

/*********************************************************************/

String Website::generateHtml() {

    String html = "";

    html += "<!DOCTYPE html><head>    <title>Apogemix mini configuration page</title></head><body>    <h3>        Apogemix mini    </h3><hr>    <p>        Current settings:        <ul>            <li>Second parachute ignition altitude: <strong>";

    html += String(glob.memory.secondSeparAltitude);
    html += "</strong> m</li>        </ul>    </p><hr>    <a href='/FlightData.apg'>Download data from last flight.</a>    <p>        Flight data table:        <table border='1'>            <tr>                <th>Num</th>                <th>Apogee [m]</th>                <th>Max speed [m/s]</th>            </tr>";
    
    for (uint8_t i = 0; i < FLIGHTS_IN_MEM; i++) {

        html += "<tr><td>" + String(glob.memory.flight[i].num) + "</td>";
        html += "<td>" + String(glob.memory.flight[i].apogee) + "</td>";
        html += "<td>" + String(glob.memory.flight[i].maxSpeed) + "</td></tr>";
    }
    
    html += "</table>    </p><hr>    <p>        Change settings:        <form><label for='setSecAlt'>Second parachute ignition altitude:</label><br><input type='number' min='50' max='5000' id='setSecAlt' name='setSecAlt'><br><br><input type='submit'><hr>        </form>    </p>    </p>    <a href='/wifioff'>Turn off the wifi.</a>    <p><hr>    </body></html>";

    return html;
}

/*********************************************************************/

void Website::handleArgs(AsyncWebServerRequest *request) {

    uint8_t paramNb = request->params();

    if (paramNb) {

        for (uint8_t i = 0; i < paramNb; i++) {

            AsyncWebParameter* p = request->getParam(i);

            // Recovery params:
            /*else if (p->name() == "sep1Mode") {

                if      (p->value() == "1") glob.memory.isSep1BeforeApog = 1;
                else if (p->value() == "0") glob.memory.isSep1BeforeApog = 0;
            }*/

            if (p->name() == "setSecAlt") {

                uint16_t val = p->value().toInt();
                if (val >= 50 && val <= 5000) glob.memory.secondSeparAltitude = val;
            }
        }
        EEPROM.put(0, glob.memory);
        EEPROM.commit();
    }
}