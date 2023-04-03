#include "Website.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void Website::start() {

    WiFi.softAP(ssid.c_str(), password);
    MDNS.begin("apogemix");

    Serial.println("server on");
    enabled = true;

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {

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

    server.on("/e_recovery", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateHtml()); // TODO
    });

    server.on("/e_telemetry", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateHtml()); // TODO
    });

    server.on("/e_servos", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateHtml()); // TODO
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

    String html = R"rawliteral(
        <!DOCTYPE html>
        <head>
            <title>Apogemix Pro configuration page</title>
        </head>
        <body>
            <h3>Apogemix Pro</h3><hr>
            <p>Current settings:<br>
        )rawliteral";

    html += "Flightdata file type: <strong>" + (glob.memory.isCsvFile ? String("csv") : String("binary")) + String("</strong><br>\n");
    html += "Pyro channel 1 mode: <strong>" + (glob.memory.isSep1BeforeApog ? String("At apogee") : String("After apogee")) + String("</strong><br>\n");
    html += "Pyro channel 2 mode: <strong>" + (glob.memory.isSep2Staging ? String("Staging") : String("Double deploy")) + String("</strong><br>\n");

    if (glob.memory.isSep2Staging) {
        html += "Staging delay: <strong>" + String(glob.memory.stagingDelay) + String("</strong>ms<br>\n");
    }
    else {
        html += "<span style='color:#808080'>Staging delay: " + String(glob.memory.stagingDelay) + String("ms</span><br>\n");
    }

    html += "Dual deploy altitude: <strong>" + String(glob.memory.secondSeparAltitude) + String("</strong>m<br>\n");
    html += "Telemetry frequency: <strong>" + String(glob.memory.loraFreqMHz) + String("</strong>MHz<br>\n");
    html += "Telemetry period: <strong>" + String(glob.memory.loraDelay_ms) + String("</strong>ms<br>\n");
    html += "Telemetry callsign: <strong>" + String(glob.memory.callsign) + String("</strong><br>\n");

    html += "Servo 1 angle (init; apogee; dual deploy): <strong>"
        + String(glob.memory.servo1Initial) + String(";") + String(glob.memory.servo1Apog) + String(";") + String(glob.memory.servo1dd)
        + String("</strong>deg<br>\n");
    html += "Servo 2 angle (init; apogee; dual deploy): <strong>"
        + String(glob.memory.servo2Initial) + String(";") + String(glob.memory.servo2Apog) + String(";") + String(glob.memory.servo2dd)
        + String("</strong>deg<br>\n");

    html += R"rawliteral(
            </p><hr>
            <a href='/FlightData.apg'>Download data from last flight.</a>
            <p>Flight data table:
            <table border='1'>
                <tr>
                    <th>Num</th><th>Apogee [m]</th><th>Max speed [m/s]</th>
                </tr>
    )rawliteral";

    for (uint8_t i = 0; i < FLIGHTS_IN_MEM; i++) {

        html += "<tr><td>" + String(glob.memory.flight[i].num) + "</td>";
        html += "<td>" + String(glob.memory.flight[i].apogee) + "</td>";
        html += "<td>" + String(glob.memory.flight[i].maxSpeed) + "</td></tr>\n";
    }

    html += R"rawliteral(
            </table>
            </p><hr>
            <p>Change settings:<br>
            <a href='/e_recovery'>Edit recovery settings.<a/><br>
            <a href='/e_telemetry'>Edit telemetry settings.<a/><br>
            <a href='/e_servos'>Edit servos settings.<a/><br>
            </p><hr>
            <a href='/wifioff'>Turn off the wifi.</a>
        </body>
        </html>
    )rawliteral";

    /*
            <form>
                <p>Save data binary (less size but, needs to be converted in ApogApka, required if the time in air could be longer than 15 min.)
                or csv (more size, but conversion not needed, good for flights <15 min.)</p>
                )rawliteral";

    // Binary od CSV file save:
    html += "<input type='radio' name='dataSave' id='bin' value='0'><label for='0'>binary,</label><br>\n";
    html += "<input type='radio' name='dataSave' id='csv' value='1'><label for='1'>csv.</label><br>\n";

    // Separation 1 pin mode:
    html += "<p>Set the separation 1 pin mode:</p>\n";
    html += "<input type='radio' name='sep1Mode' id='after' value='0'><label for='after'>fire just after the apogee (safer).</label><br>\n";
    html += "<input type='radio' name='sep1Mode' id='apog' value='1'><label for='apog'>fire at the apogee,</label><br>\n";

    // Separation 2 pin modes:
    html += "<p>Set the separation 2 pin mode (dual deploy at descent or second stage ignite at ascent):</p>\n";
    html += "<input type='radio' name='sep2Mode' id='dd' value='0'><label for='dd'>dual deploy at descent (set the right second parachute ignition altitude, please),</label><br>\n";
    html += "<input type='radio' name='sep2Mode' id='stage' value='1'><label for='stage'>staging at ascent (set the right delay between launch and staging, please).</label><br>\n";
    html += "<label for='setStaging'>Delay between launch and staging (Current: <strong>"
        + String(glob.memory.secondSeparAltitude) + "</strong>ms):</label><br>\n";
    html += "<input type='number' min='2000' max='30000' name='setStaging'><br>\n";
    html += "<label for='setSecAlt'>Second parachute ignition altitude (Current: <strong>"
        + String(glob.memory.secondSeparAltitude) + "</strong>m):</label><br>\n";
    html += "<input type='number' min='50' max='5000' name='setSecAlt'><br>\n";

    // Transmitter modes:
    html += "<label for='setLoraFreq'>LoRa frequency (Current: <strong>"
        + String(glob.memory.loraFreqMHz) + "</strong>MHz):</label><br>\n";
    html += "<input type='number' min='100' max='999' name='setLoraFreq'><br>\n";
    html += "<label for='setLoRaDelay'>LoRa send period (Current: <strong>"
        + String(glob.memory.loraDelay_ms) + "</strong>ms):</label><br>\n";
    html += "<input type='number' min='500' max='5000' name='setLoRaDelay'><br>\n";
    html += "<label for='setCallSign'>LoRa send period (Current: <strong>"
        + String(glob.memory.callsign) + "</strong>):</label><br>\n";
    html += "<input type='text' maxlength='10' name='setCallSign'><br>\n";

    // Servo 1 modes:
    html += "<label for='servo1init'>Initial servo 1 angle (Current: <strong>"
        + String(glob.memory.servo1Initial) + "</strong>deg):</label><br>\n";
    html += "<input type='number' min='0' max='180' name='servo1init'><br>\n";
    html += "<label for='servo1Apog'>Apogee servo 1 angle (Current: <strong>"
        + String(glob.memory.servo1Apog) + "</strong>deg):</label><br>\n";
    html += "<input type='number' min='0' max='180' name='servo1Apog'><br>\n";
    html += "<label for='servo1dd'>Second deploy servo 1 angle (Current: <strong>"
        + String(glob.memory.servo1dd) + "</strong>deg):</label><br>\n";
    html += "<input type='number' min='0' max='180' name='servo1dd'><br>\n";

    // Servo 2 modes:
    html += "<label for='servo2init'>Initial servo 2 angle (Current: <strong>"
        + String(glob.memory.servo2Initial) + "</strong>deg):</label><br>\n";
    html += "<input type='number' min='0' max='180' name='servo2init'><br>\n";
    html += "<label for='servo2Apog'>Apogee servo 2 angle (Current: <strong>"
        + String(glob.memory.servo2Apog) + "</strong>deg):</label><br>\n";
    html += "<input type='number' min='0' max='180' name='servo2Apog'><br>\n";
    html += "<label for='servo2dd'>Second deploy servo 2 angle (Current: <strong>"
        + String(glob.memory.servo2dd) + "</strong>deg):</label><br>\n";
    html += "<input type='number' min='0' max='180' name='servo2dd'><br>\n";
    */

    return html;
}

/*********************************************************************/

void Website::handleArgs(AsyncWebServerRequest *request) {

    if (request->params()) {

        // TODO dodać dataSave
        // TODO sep1Mode
        // TODO sep2Mode
        // TODO setStaging
        uint16_t setSecAlt    = request->getParam("setSecAlt")->value().toInt();
        uint16_t setLoraFreq  = request->getParam("setLoraFreq")->value().toInt();
        uint16_t setLoRaDelay = request->getParam("setLoRaDelay")->value().toInt();
        // TODO setCallSign
        // TODO servo1init
        // TODO servo1Apog
        // TODO servo1dd
        // TODO servo2init
        // TODO servo2Apog
        // TODO servo2dd

        if (setSecAlt >= 50 && setSecAlt <= 5000) glob.memory.secondSeparAltitude = setSecAlt;
        if (setLoraFreq >= 100 && setLoraFreq <= 999) glob.memory.loraFreqMHz     = setLoraFreq;
        if (setLoRaDelay >= 500 && setLoRaDelay <= 5000) glob.memory.loraDelay_ms = setLoRaDelay;

        EEPROM.put(0, glob.memory);
        EEPROM.commit();
    }
}