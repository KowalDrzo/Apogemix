const websocket = new WebSocket("ws://192.168.4.1/ws");

websocket.addEventListener("message", (event) => {
    const receivedData = event.data;
    parseFrame(receivedData);
});

setTimeout(getDevices, 1000);

function selectCommand() {

    const command = document.querySelector("#commandList").value;
    sendCommand(command);
}

function sendCommand(command) {

    const select = document.getElementById('deviceList');
    const selectedDevice = select.value;

    if (!selectedDevice) {
        return;
    }

    let dataToSend = selectedDevice + ";" + command;

    if (confirm("Are you sure to send " + command + " command?")) {
        websocket.send(dataToSend);
    }
}

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
    setTimeout(getDevices, 6000);
}

function parseFrame(receivedData) {

    const select = document.getElementById('deviceList');
    const selectedDevice = select.value;
    const frameData = receivedData.split(';');

    if (!selectedDevice) {
        return;
    }
    if (frameData[0] != selectedDevice) {
        return;
    }

    document.querySelector("#gpsLat").textContent = frameData[1];
    document.querySelector("#gpsLng").textContent = frameData[2];
    document.querySelector("#gpsAlt").textContent = frameData[3];
    document.querySelector("#press").textContent = frameData[6];
    document.querySelector("#altitude").textContent = frameData[7];
    document.querySelector("#speed").textContent = frameData[8];
    document.querySelector("#temper").textContent = frameData[5];
    document.querySelector("#time_ms").textContent = frameData[4];

    const continuities = frameData[9];
    var cont1 = document.getElementById("cont1");
    var cont2 = document.getElementById("cont2");
    var mosState = document.getElementById("mosState");
    if (continuities & 1) cont1.style.backgroundColor = "#005c00";
    else cont1.style.backgroundColor = "#FF0000";
    if (continuities & 2) cont2.style.backgroundColor = "#005c00";
    else cont2.style.backgroundColor = "#FF0000";
    if (continuities & 4) mosState.style.backgroundColor = "#005c00";
    else mosState.style.backgroundColor = "#FF0000";

    const state = frameData[10];
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
}
