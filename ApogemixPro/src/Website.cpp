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
        request->send(200, "text/html", generateSettingsPage(RECOVERY_SETTINGS));
    });

    server.on("/e_telemetry", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateSettingsPage(TELEMETRY_SETTINGS));
    });

    server.on("/e_servos", HTTP_GET, [this](AsyncWebServerRequest *request) {

        handleArgs(request);
        request->send(200, "text/html", generateSettingsPage(SERVOS_SETTINGS));
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

        if (glob.memory.flight[i].apogee || glob.memory.flight[i].maxSpeed) {
            html += "<tr><td>" + String(glob.memory.flight[i].num) + "</td>";
            html += "<td>" + String(glob.memory.flight[i].apogee) + "</td>";
            html += "<td>" + String(glob.memory.flight[i].maxSpeed) + "</td></tr>\n";
        }
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

    return html;
}

/*********************************************************************/

String Website::generateSettingsPage(uint8_t settingsType) {

    String html = R"rawliteral(
    <!DOCTYPE html>
    <head>
        <title>Apogemix Pro configuration page</title>
    </head>
    <body>
        <h3>Settings for )rawliteral";

    switch (settingsType) {

    case RECOVERY_SETTINGS:

        html += "recovery</h3>\n\t\t<form>\n";

        // Binary or CSV file save:
        html += "<p>Save data binary (less size but, needs to be converted in ApogApka, required if the time in air could be longer than 15 min.)\
            or csv (more size, but conversion not needed, good for flights <15 min.)</p>\n";
        if (glob.memory.isCsvFile) {
            html += "<input type='radio' name='dataSave' id='bin' value='0'><label for='0'>binary,</label><br>\n";
            html += "<input type='radio' name='dataSave' id='csv' value='1' checked><label for='1'>csv.</label><br>\n";
        }
        else {
            html += "<input type='radio' name='dataSave' id='bin' value='0' checked><label for='0'>binary,</label><br>\n";
            html += "<input type='radio' name='dataSave' id='csv' value='1'><label for='1'>csv.</label><br>\n";
        }

        // Separation 1 pin mode:
        html += "<p>Set the separation 1 pin mode:</p>\n";
        if (glob.memory.isSep1BeforeApog) {
            html += "<input type='radio' name='sep1Mode' id='after' value='0'><label for='after'>fire just after the apogee (safer).</label><br>\n";
            html += "<input type='radio' name='sep1Mode' id='apog' value='1' checked><label for='apog'>fire at the apogee,</label><br>\n";
        }
        else {
            html += "<input type='radio' name='sep1Mode' id='after' value='0' checked><label for='after'>fire just after the apogee (safer).</label><br>\n";
            html += "<input type='radio' name='sep1Mode' id='apog' value='1'><label for='apog'>fire at the apogee,</label><br>\n";
        }

        // Separation 2 pin modes:
        html += "<p>Set the separation 2 pin mode (dual deploy at descent or second stage ignite at ascent):</p>\n";
        if (glob.memory.isSep2Staging) {
            html += "<input type='radio' name='sep2Mode' id='dd' value='0'><label for='dd'>dual deploy at descent (set the right second parachute ignition altitude, please),</label><br>\n";
            html += "<input type='radio' name='sep2Mode' id='stage' value='1' checked><label for='stage'>staging at ascent (set the right delay between launch and staging, please).</label><br>\n";
        }
        else {
            html += "<input type='radio' name='sep2Mode' id='dd' value='0' checked><label for='dd'>dual deploy at descent (set the right second parachute ignition altitude, please),</label><br>\n";
            html += "<input type='radio' name='sep2Mode' id='stage' value='1'><label for='stage'>staging at ascent (set the right delay between launch and staging, please).</label><br>\n";
        }
        html += "<label for='setStaging'>Delay between launch and staging (Current: <strong>"
            + String(glob.memory.stagingDelay) + "</strong>ms):</label><br>\n";
        html += "<input type='number' min='2000' max='30000' name='setStaging'><br>\n";
        html += "<label for='setSecAlt'>Second parachute ignition altitude (Current: <strong>"
            + String(glob.memory.secondSeparAltitude) + "</strong>m):</label><br>\n";
        html += "<input type='number' min='50' max='5000' name='setSecAlt'><br>\n";

        break;

    /******************************/
    case TELEMETRY_SETTINGS:

        html += "telemetry</h3>\n\t\t<form>\n";

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

        break;

    /******************************/
    case SERVOS_SETTINGS:

        html += "servos</h3>\n\t\t<form>\n";

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

        break;
    }

    html += R"rawliteral(
            <input type="submit" value="save">
        </form>
        <a href='/'>Back to main page.<a/>
    </body>
    </html>

    )rawliteral";

    return html;
}

/*********************************************************************/

void Website::handleArgs(AsyncWebServerRequest *request) {

    uint8_t paramNb = request->params();

    if (paramNb) {

        for (uint8_t i = 0; i < paramNb; i++) {

            AsyncWebParameter* p = request->getParam(i);

            // Recovery params:
            if (p->name() == "dataSave") {

                if      (p->value() == "1") glob.memory.isCsvFile = 1;
                else if (p->value() == "0") glob.memory.isCsvFile = 0;
            }

            else if (p->name() == "sep1Mode") {

                if      (p->value() == "1") glob.memory.isSep1BeforeApog = 1;
                else if (p->value() == "0") glob.memory.isSep1BeforeApog = 0;
            }

            else if (p->name() == "sep2Mode") {

                if      (p->value() == "1") glob.memory.isSep2Staging = 1;
                else if (p->value() == "0") glob.memory.isSep2Staging = 0;
            }

            else if (p->name() == "setStaging") {

                uint16_t val = p->value().toInt();
                if (val >= 2000 && val <= 30000) glob.memory.stagingDelay = val;
            }

            else if (p->name() == "setSecAlt") {

                uint16_t val = p->value().toInt();
                if (val >= 50 && val <= 5000) glob.memory.secondSeparAltitude = val;
            }

            // Telemetry params:
            else if (p->name() == "setLoraFreq") {

                uint16_t val = p->value().toInt();
                if (val >= 100 && val <= 999) glob.memory.loraFreqMHz = val;
            }

            else if (p->name() == "setLoRaDelay") {

                uint16_t val = p->value().toInt();
                if (val >= 500 && val <= 5000) glob.memory.loraDelay_ms = val;
            }

            else if (p->name() == "setCallSign" && p->value().length() >= 2) {

                String newCall = p->value();
                strncpy(glob.memory.callsign, newCall.c_str(), CALLSIGN_LEN - 1);
                glob.memory.callsign[CALLSIGN_LEN - 1] = 0;
            }

            // Servo params:
            else if (p->name() == "servo1init") {

                uint8_t val = p->value().toInt();
                if (val >= 0 && val <= 180) glob.memory.servo1Initial = val;
            }

            else if (p->name() == "servo1Apog") {

                uint8_t val = p->value().toInt();
                if (val >= 0 && val <= 180) glob.memory.servo1Apog = val;
            }

            else if (p->name() == "servo1dd") {

                uint8_t val = p->value().toInt();
                if (val >= 0 && val <= 180) glob.memory.servo1dd = val;
            }

            else if (p->name() == "servo2init") {

                uint8_t val = p->value().toInt();
                if (val >= 0 && val <= 180) glob.memory.servo2Initial = val;
            }

            else if (p->name() == "servo2Apog") {

                uint8_t val = p->value().toInt();
                if (val >= 0 && val <= 180) glob.memory.servo2Apog = val;
            }

            else if (p->name() == "servo2dd") {

                uint8_t val = p->value().toInt();
                if (val >= 0 && val <= 180) glob.memory.servo2dd = val;
            }
        }
        EEPROM.put(0, glob.memory);
        EEPROM.commit();
    }
}