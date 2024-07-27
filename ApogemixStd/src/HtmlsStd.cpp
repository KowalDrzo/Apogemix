#include "Website.h"

#ifndef PRO

String Website::generateHtml() {

    String html = R"rawliteral(
        <!DOCTYPE html>
        <head>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>Apogemix Standard configuration page</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    margin: 0;
                    padding: 0;
                    background-color: #f0f2f5;
                }
                .container {
                    max-width: 800px;
                    margin: 0 auto;
                    padding: 20px;
                    background-color: #fff;
                    border-radius: 5px;
                    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
                }
                h3 {
                    text-align: center;
                    margin-top: 20px;
                    color: #333;
                }
                hr {
                    border: 1px solid #ccc;
                    margin: 20px 0;
                }
                .block {
                    margin-bottom: 20px;
                    padding: 10px;
                    border: 1px solid #ccc;
                    border-radius: 5px;
                    background-color: #f9f9f9;
                }
                .button {
                    display: inline-block;
                    padding: 10px 20px;
                    background-color: #007bff;
                    color: #fff;
                    border: none;
                    border-radius: 5px;
                    cursor: pointer;
                    text-decoration: none; /* Usunięcie podkreślenia */
                }
                .button:hover {
                    background-color: #0056b3;
                }
                .gray-text {
                    color: #808080;
                }
                .button-group {
                    display: flex;
                    justify-content: space-between;
                    margin-top: 10px;
                }
            </style>
        </head>
        <body>
            <div class="container">
                <h3>Apogemix Standard</h3>
                <hr>
                <div class="block">
                    <p class="gray-text">Current settings:</p>
        )rawliteral";

    html += "Flightdata file type: <strong>" + (glob.memory.isCsvFile ? String("csv") : String("binary")) + String("</strong><br>\n");
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

    html += R"rawliteral(
            </div>
                <div class="block">
                    <a class="button" href="/FlightData.apg">Download data from last flight</a>
                </div>
                <div class="block">
                    <p class="gray-text">Flight data table:</p>
                    <table>
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
                </div>
                <div class="block">
                    <p class="gray-text">Change settings:</p>
                    <div class="button-group">
                        <a class="button" href="/e_recovery">Edit recovery settings</a>
                        <a class="button" href="/e_telemetry">Edit telemetry settings</a>
                        <a class="button" href="/recovery_test">Test pyro or servos</a>
                    </div>
                </div>
                <div class="block">
                    <a class="button" href="/wifioff">Turn off the wifi</a>
                </div>
            </div>
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
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>Apogemix Standard configuration page</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    margin: 0;
                    padding: 0;
                    background-color: #f0f2f5;
                }
                .container {
                    max-width: 800px;
                    margin: 0 auto;
                    padding: 20px;
                    background-color: #fff;
                    border-radius: 5px;
                    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
                }
                h3 {
                    text-align: center;
                    margin-top: 20px;
                    color: #333;
                }
                hr {
                    border: 1px solid #ccc;
                    margin: 20px 0;
                }
                .block {
                    margin-bottom: 20px;
                    padding: 10px;
                    border: 1px solid #ccc;
                    border-radius: 5px;
                    background-color: #f9f9f9;
                }
                .button {
                    display: inline-block;
                    padding: 10px 20px;
                    background-color: #007bff;
                    color: #fff;
                    border: none;
                    border-radius: 5px;
                    cursor: pointer;
                    text-decoration: none;
                }
                .button:hover {
                    background-color: #0056b3;
                }
                input[type="number"],
                input[type="text"],
                input[type="submit"] {
                    margin-top: 5px;
                    padding: 5px;
                    border: 1px solid #ccc;
                    border-radius: 5px;
                }
                .gray-text {
                    color: #808080;
                }
            </style>
        </head>
        <body>
            <div class="container">
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
        break;
    }

    html += R"rawliteral(
            <input type="submit" class="button" value="save">
            </form>
            <a href="/" class="button">Back to main page</a>
        </div>
    </body>
    </html>

    )rawliteral";

    return html;
}

/*********************************************************************/

String Website::generateRecoveryTest() {

    String html = R"rawliteral(
        <!DOCTYPE html>
        <head>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>Apogemix Standard configuration page</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    margin: 0;
                    padding: 0;
                    background-color: #f0f2f5;
                }
                .container {
                    max-width: 800px;
                    margin: 0 auto;
                    padding: 20px;
                    background-color: #fff;
                    border-radius: 5px;
                    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
                }
                h3 {
                    text-align: center;
                    margin-top: 20px;
                    color: #333;
                }
                hr {
                    border: 1px solid #ccc;
                    margin: 20px 0;
                }
                .block {
                    margin-bottom: 20px;
                    padding: 10px;
                    border: 1px solid #ccc;
                    border-radius: 5px;
                    background-color: #f9f9f9;
                }
                .button {
                    display: inline-block;
                    padding: 10px 20px;
                    background-color: #007bff;
                    color: #fff;
                    border: none;
                    border-radius: 5px;
                    cursor: pointer;
                    text-decoration: none;
                }
                .button:hover {
                    background-color: #0056b3;
                }
                input[type="number"],
                input[type="text"],
                input[type="submit"] {
                    margin-top: 5px;
                    padding: 5px;
                    border: 1px solid #ccc;
                    border-radius: 5px;
                }
                .gray-text {
                    color: #808080;
                }
            </style>
        </head>
        <body>
            <div class="container">
                <h3>Recovery testing</h3><hr>
                <button class="button" id="test1Button">Test pyro 1</button>
                <button class="button" id="test2Button">Test pyro 2</button>
                <hr>

                <script>
                    function confirmAction(message) {
                        return confirm(message);
                    }

                    document.getElementById("test1Button").addEventListener("click", function() {
                        if (confirmAction("Are you sure to fire channel 1?")) {
                            fetch("/recovery_test1", {
                                method: "POST"
                            });
                        }
                    });

                    document.getElementById("test2Button").addEventListener("click", function() {
                        if (confirmAction("Are you sure to fire channel 2?")) {
                            fetch("/recovery_test2", {
                                method: "POST"
                            });
                        }
                    });
                </script>
            <a href="/" class="button">Back to main page</a>
        </div>
    </body>
    </html>

    )rawliteral";

    return html;
}

#endif
