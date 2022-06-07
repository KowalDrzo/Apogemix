#include "Website.h"

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

    server.begin();
}

/*********************************************************************/

void Website::stop() {

    Serial.println("server off");
    WiFi.mode(WIFI_OFF);
    enabled = false;
}

/*********************************************************************/

String Website::generateHtml() {

    String html = "";

    html += "<!DOCTYPE html><head>    <title>Apogemix mini configuration page</title></head><body>    <h3>        Apogemix mini    </h3><hr>    <p>        Current settings:        <ul>            <li>Apogee ignite delay: <strong>";
    
    html += String(glob.memory.firstSeparDelay_ms);
    html += "</strong> ms,</li>            <li>Second parachute ignition altitude: <strong>";
    html += String(glob.memory.secondSeparAltitude);
    html += "</strong> m,</li>            <li>Wifi active time: <strong>";
    html += String(glob.memory.wifiActiveTime_min);
    html += "</strong> min,</li>        </ul>    </p><hr>    <a href='/FlightData.apg'>Download data from last flight.</a>    <p>        Flight data table:        <table border='1'>            <tr>                <th>Num</th>                <th>Apogee [m]</th>                <th>Max speed [m/s]</th>            </tr>";
    
    for (uint8_t i = 0; i < FLIGHTS_IN_MEM; i++) {

        html += "<tr><td>" + String(glob.memory.flight[i].num) + "</td>";
        html += "<td>" + String(glob.memory.flight[i].apogee) + "</td>";
        html += "<td>" + String(glob.memory.flight[i].maxSpeed) + "</td></tr>";
    }
    
    html += "</table>    </p><hr>    <p>        Change settings:        <form>            <label for='set1'>Apogee ignite delay:</label><br>            <input type='number' min='1' max='5000' id='set1' name='set1'><br>            <label for='set2'>Second parachute ignition altitude:</label><br>            <input type='number' min='50' max='5000' id='set2' name='set2'><br>            <label for='set3'>Wifi active time:</label><br>            <input type='number' min='2' max='15' id='set3' name='set3'><br>            <br><input type='submit'><hr>        </form>    </p>    </p>    <a href='/wifioff'>Turn off the wifi.</a>    <p><hr>    </body></html>";

    return html;
}

/*********************************************************************/

void Website::handleArgs(AsyncWebServerRequest *request) {

    if (request->hasParam("set1") && request->hasParam("set2") && request->hasParam("set3")) {

        uint8_t set1 = request->getParam("set1")->value().toInt();
        uint8_t set2 = request->getParam("set2")->value().toInt();
        uint8_t set3 = request->getParam("set3")->value().toInt();

        if (set1 >= 1 && set1 <= 5000) glob.memory.firstSeparDelay_ms   = set1;
        if (set2 >= 50 && set2 <= 5000) glob.memory.secondSeparAltitude = set2;
        if (set3 >= 2 && set3 <= 15) glob.memory.wifiActiveTime_min     = set3;

        EEPROM.put(0, glob.memory);
        EEPROM.commit();
    }
}