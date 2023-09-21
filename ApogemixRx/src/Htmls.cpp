#include "Website.h"

String Website::generateHtml() {

    String html = R"rawliteral(

        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Apogemix Receive Station</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    display: grid;
                    grid-template-columns: 1fr;
                    grid-gap: 20px;
                    padding: 20px;
                }

                .tile_container {
                    display: grid;
                    grid-template-columns: repeat(3, 1fr);
                    grid-gap: 20px;
                }

                @media (max-width: 768px) {
                    .tile_container {
                        grid-template-columns: 1fr;
                    }
                }

                .tile {
                    display: flex;
                    flex-direction: column;
                    background-color: #f0f0f0;
                    padding: 20px;
                    border: 1px solid #ccc;
                    border-radius: 5px;
                    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
                    text-align: center;
                    justify-content: center;
                    align-items: center;
                }

                h1 {
                    font-size: 28px;
                    font-weight: bold;
                }

                h2 {
                    font-size: 18px;
                }

                p.placeholder {
                    font-size: 28px;
                    font-weight: bold;
                }

                .toggle-label {
                    display: flex;
                    align-items: center;
                    margin-top: 5px;
                }

                .toggle-label {
                    margin-bottom: 10px;
                }

                .button {
                    background-color: #007BFF;
                    color: #fff;
                    border: none;
                    padding: 10px 20px;
                    border-radius: 5px;
                    cursor: pointer;
                    font-size: 18px; /* Domyślnie większy przycisk */
                    margin: 10px;
                    width: fit-content;
                }

                .progressbar {
                    height: 20px;
                    width: 100%;
                    background-color: #eee;
                    border-radius: 5px;
                    margin-top: 10px;
                }

                .progressbar_fill {
                    height: 100%;
                    background-color: #007BFF;
                    border-radius: 5px;
                    width: 50%;
                }

                .led {
                    font-weight: bold;
                    color: #eee;
                    background-color: #FFFFFF;
                    padding: 10px;
                    display: inline-block;
                    margin-top: 10px;
                }
            </style>
        </head>
        <body>
            <div class="tile">
                <h1>Apogemix Receive Station</h1>
                <form>
                    <label for="freqmhz">Frequency MHz:</label>
                    <input type="text" id="freqmhz" name="freqmhz">
                    <button type="submit" class="button">Accept</button>
                </form>
                <h2>Devices List</h2>
                <select id="deviceList">
                </select>
            </div>

            <div class="tile_container">
                <div class="tile">
                    <h2>GPS lat</h2>
                    <p class="placeholder" id="gpsLat">+NN.NNNN</p>
                </div>
                <div class="tile">
                    <h2>GPS lng</h2>
                    <p class="placeholder" id="gpsLng">+EEE.EEEE</p>
                </div>
                <div class="tile">
                    <h2>GPS altitude</h2>
                    <p class="placeholder" id="gpsAlt">AAAAA.A</p>
                </div>
                <div class="tile">
                    <h2>Pressure</h2>
                    <p class="placeholder" id="press">PPPPPP.P</p>
                </div>
                <div class="tile">
                    <h2>Baro altitude</h2>
                    <p class="placeholder" id="altitude">AAAAA.A</p>
                </div>
                <div class="tile">
                    <h2>Baro speed</h2>
                    <p class="placeholder" id="speed">SSSS.S</p>
                </div>
                <div class="tile">
                    <h2>Temperature</h2>
                    <p class="placeholder" id="temper">+TT.T</p>
                </div>
                <div class="tile">
                    <h2>Time [ms]</h2>
                    <p class="placeholder" id="time_ms">TTTTTTTT</p>
                </div>
                <div class="tile">
                    <div id="cont1" class="led">Continuity 1</div>
                    <div id="cont2" class="led">Continuity 2</div>
                    <div id="mosState" class="led">Mosfet</div>
                </div>
                <div class="tile">
                    <button class="button">Test 1</button>
                    <button class="button">Test 2</button>
                </div>
                <div class="tile">
                    <select id="commandList">
                        <option value="MOS_ON">MOS_ON</option>
                        <option value="MOS_OFF">MOS_OFF</option>
                        <option value="MOS_CLK">MOS_CLK</option>
                        <option value="RECALIBRATE">RECALIBRATE</option>
                    </select>
                    <button class="button">Send command</button>
                </div>
                <div class="tile">
                    <label class="label">Rocket State:</label>
                    <label class="label" id="rocketState">Text</label>
                    <div class="progressbar">
                        <div class="progressbar_fill" id="progressbar_fill"></div>
                    </div>
                </div>
            </div>
            <a href="/wifioff">Turn off WiFi.</a>

            <script>
                function getDevices() {

                    fetch('/devices')
                        .then(response => response.text())
                        .then(data => {
                            const devicesList = data.split(';');
                            console.log(devicesList);
                            const select = document.getElementById('deviceList');

                            devicesList.forEach(device => {

                                var deviceOnList = false;
                                for (var i = 0; i < select.options.length; i++) {
                                    if (select.options[i].text === device) {
                                    deviceOnList = true;
                                    break;
                                    }
                                }

                                if (!deviceOnList) {
                                    const option = document.createElement('option');
                                    option.text = device;
                                    select.add(option);
                                }
                            });
                        });
                }

                function getFrame() {

                    const select = document.getElementById('deviceList');
                    const selectedDevice = select.value;

                    if (!selectedDevice) {
                        return;
                    }

                    fetch(`/frame?device=${selectedDevice}`)
                        .then(response => response.text())
                        .then(data => {

                            const frameData = data.split(';');

                            const gpsLat = document.getElementById('gpsLat');
                            gpsLat.textContent = frameData[0];
                            const gpsLng = document.getElementById('gpsLng');
                            gpsLng.textContent = frameData[1];
                            const gpsAlt = document.getElementById('gpsAlt');
                            gpsAlt.textContent = frameData[2];

                            const press = document.getElementById('press');
                            press.textContent = frameData[5];
                            const altitude = document.getElementById('altitude');
                            altitude.textContent = frameData[6];
                            const speed = document.getElementById('speed');
                            speed.textContent = frameData[7];

                            const temper = document.getElementById('temper');
                            temper.textContent = frameData[4];
                            const time_ms = document.getElementById('time_ms');
                            time_ms.textContent = frameData[3];

                            const continuities = frameData[8];
                            var cont1 = document.getElementById("cont1");
                            var cont2 = document.getElementById("cont2");
                            var mosState = document.getElementById("mosState");
                            if (continuities & 1) cont1.style.backgroundColor = "#005c00";
                            else cont1.style.backgroundColor = "#FF0000";
                            if (continuities & 2) cont2.style.backgroundColor = "#005c00";
                            else cont2.style.backgroundColor = "#FF0000";
                            if (continuities & 4) mosState.style.backgroundColor = "#005c00";
                            else mosState.style.backgroundColor = "#FF0000";

                            const state = frameData[9];
                            var rocketState = document.getElementById("rocketState");
                            var progressbar_fill = document.getElementById("progressbar_fill");
                            if (state == 0) {
                                rocketState.innerHTML = "On rail";
                                progressbar_fill.style.width = "10%";
                            }
                            else if (state == 1) {
                                rocketState.innerHTML = "Flight";
                                progressbar_fill.style.width = "25%";
                            }
                            else if (state == 2) {
                                rocketState.innerHTML = "First separation";
                                progressbar_fill.style.width = "50%";
                            }
                            else if (state == 3) {
                                rocketState.innerHTML = "Second separation";
                                progressbar_fill.style.width = "75%";
                            }
                            else if (state == 4) {
                                rocketState.innerHTML = "On ground";
                                progressbar_fill.style.width = "100%";
                            }

                        });
                }

                setInterval(getDevices, 3000);
                setInterval(getFrame, 1000);
            </script>
        </body>
        </html>

    )rawliteral";

    return html;
}
