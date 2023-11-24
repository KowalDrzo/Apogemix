#include "Website.h"

String Website::generateHtml() {

    String html = R"rawliteral(
        <!DOCTYPE html>
        <head>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>Apogemix Mini configuration page</title>
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
                <h3>Apogemix Mini</h3>
                <hr>
                <div class="block">
                    <p class="gray-text">Current settings:</p>
        )rawliteral";

    html += "Pyro channel 1 mode: <strong>" + (glob.memory.isSep1BeforeApog ? String("At apogee") : String("After apogee")) + String("</strong><br>\n");
    html += "Dual deploy altitude: <strong>" + String(glob.memory.secondSeparAltitude) + String("</strong>m<br>\n");

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
            <title>Apogemix Pro configuration page</title>
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
        html += "<label for='setSecAlt'>Second parachute ignition altitude (Current: <strong>"
            + String(glob.memory.secondSeparAltitude) + "</strong>m):</label><br>\n";
        html += "<input type='number' min='50' max='5000' name='setSecAlt'><br>\n";

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
            <title>Apogemix Pro configuration page</title>
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
