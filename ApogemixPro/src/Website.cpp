#include "Website.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void Website::start() {

    WiFi.softAP(ssid.c_str(), password);
    MDNS.begin("apogemix");

    Serial.println("server on");
    enabled = true;

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateHtml());
    });

    server.on("/api", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", glob.dataFrame.toString());
    });

    server.on("/wifioff", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(200, "text/html", String("<meta http-equiv=\"refresh\" content=\"0; URL=/\" />"));
        stop();
    });

    server.on("/FlightData.apg", HTTP_GET, [this](AsyncWebServerRequest *request) {

        request->send(LITTLEFS, "/FlightData.apg", String());
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

    html += "<!DOCTYPE html><head>    <title>Apogemix mini configuration page</title></head><body>    <h3>        Apogemix mini    </h3><hr>    <p>        Current settings:        <ul>            <li>Apogee ignite delay: <strong>";
    
    html += String(glob.memory.firstSeparDelay_ms);
    html += "</strong> ms,</li> <li>Second parachute ignition altitude: <strong>";
    html += String(glob.memory.secondSeparAltitude);
    html += "</strong> m,</li> <li>Wifi active time: <strong>";
    html += String(glob.memory.wifiActiveTime_min);
    html += "</strong> min,</li> <li>LoRa frequency: <strong>";
    html += String(glob.memory.loraFreqMHz);
    html += "</strong> MHz,</li> <li>LoRa send data period: <strong>";
    html += String(glob.memory.loraDelay_ms);
    html += "</strong> ms.</li>";
    html += "</ul>    </p><hr>    <a href='/FlightData.apg'>Download data from last flight.</a>    <p>        Flight data table:        <table border='1'>            <tr>                <th>Num</th>                <th>Apogee [m]</th>                <th>Max speed [m/s]</th>            </tr>";
    
    for (uint8_t i = 0; i < FLIGHTS_IN_MEM; i++) {

        html += "<tr><td>" + String(glob.memory.flight[i].num) + "</td>";
        html += "<td>" + String(glob.memory.flight[i].apogee) + "</td>";
        html += "<td>" + String(glob.memory.flight[i].maxSpeed) + "</td></tr>";
    }
    
    html += "</table>    </p><hr>    <p>        Change settings:        <form>            <label for='set1'>Apogee ignite delay:</label><br>            <input type='number' min='1' max='5000' id='set1' name='set1'><br>            <label for='set2'>Second parachute ignition altitude:</label><br>            <input type='number' min='50' max='5000' id='set2' name='set2'><br>            <label for='set3'>Wifi active time:</label><br>            <input type='number' min='2' max='15' id='set3' name='set3'><br>";
    html += "<label for='set4'>LoRa frequency:</label><br> <input type='number' min='100' max='999' id='set4' name='set4'><br>";
    html += "<label for='set5'>LoRa send period:</label><br> <input type='number' min='500' max='5000' id='set5' name='set5'><br>";
    html +="<br><input type='submit'><hr>        </form>    </p>    </p>    <a href='/wifioff'>Turn off the wifi.</a>    <p><hr>    </body></html>";

    return html;
}

/*********************************************************************/

void Website::handleArgs(AsyncWebServerRequest *request) {

    if (request->hasParam("set1") && request->hasParam("set2") && request->hasParam("set3")) {

        uint16_t set1 = request->getParam("set1")->value().toInt();
        uint16_t set2 = request->getParam("set2")->value().toInt();
        uint8_t set3 = request->getParam("set3")->value().toInt();
        uint16_t set4 = request->getParam("set4")->value().toInt();
        uint16_t set5 = request->getParam("set5")->value().toInt();

        if (set1 >= 1 && set1 <= 5000) glob.memory.firstSeparDelay_ms   = set1;
        if (set2 >= 50 && set2 <= 5000) glob.memory.secondSeparAltitude = set2;
        if (set3 >= 2 && set3 <= 15) glob.memory.wifiActiveTime_min     = set3;
        if (set4 >= 100 && set4 <= 999) glob.memory.loraFreqMHz         = set4;
        if (set5 >= 500 && set3 <= 5000) glob.memory.loraDelay_ms       = set5;

        EEPROM.put(0, glob.memory);
        EEPROM.commit();
    }
}