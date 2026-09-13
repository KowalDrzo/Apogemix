# Apogemix

### Wi-Fi flight computer for experimental rockets

**Apogemix** is an flight control and telemetry system designed to bring modern connectivity, flight monitoring and automation to experimental rockets.

**Bring Wi-Fi altimetry into your rocket.**

The project is designed with experimental rocketry in mind — from small educational rockets to more advanced vehicles requiring reliable flight data, telemetry and recovery-system control.

At its core, Apogemix uses **a single barometric pressure sensor** to understand what is happening to the rocket during flight. Without requiring an accelerometer, gyroscope or other inertial sensors, it can detect the key phases of a flight, control recovery-related outputs and record flight data.

Depending on the hardware version, Apogemix can also be equipped with **GPS and LoRa**, adding long-range telemetry and live location tracking to the system.

---

## Devices

Apogemix software is compatible with Apogemix hardware listed below:

- Apogemix Mini,
- Apogemix Shield - *test version, discontinued*,
- Apogemix Pro / Standard / Maxi - *test versions, discontinued*,
- Apogemix JP30.

## Existing features

- detect flight states (RAIL, LAUNCH, APOGEE, DUAL_DEPLOY, LANDING),
- fire e-matches using 2 mosfet channels,
- save flight data (altitude vs time),
- send GPS location to the ground station using LoRa (GPS & LoRa versions only),
- perform recovery test from safe distance using WiFi or LoRa,
- set configuration using WiFi.

The core flight functionality works using **only the pressure sensor**. GPS and LoRa are optional features available on appropriate Apogemix variants.

## Future features

- servo control,
- more simple settings for LoRa,
- password for LoRa recovery test,
- motor ignition mode (remote ignitor instead of rocketry computer),
- magic number in EEPROM memory (to apply default setting after first device startup),
- option in setting to run WiFi automatically or after button (both Rocket and Rx),
- mah delay for supersonic flights.

## Repo content

- `ApogemixRocket` - project for Apogemix boards which are designed for rockets. Based on ESP32-C3,
- `ApogemixRx` - project for Apogemix ground stations. based on ESP32-C3.

## Usage

### Power and wiring

The Apogemix should be powered in the way that could be able to deliver enough power and be switched on when the rocket is on rail. I strongly suggest using key switches or other switches accessible from the rocket airframe. Powering on (arming) a rocket while assembly is dangerous.

For the voltage it is best to use fully charged lipo 1s (> 1000 mAh, > 4,1V) or charged lipo 2s (> 250 mAh, > 8V).

The liion 14500 cells should be avoided due to their high internal resistance. Liion 18650 and 21700 can be used but only the high current versions (tabless cells).

### WiFi connection

Apogemix uses ESP32-C3 microcontroller which has a 2.4 GHz WiFi module built-in. WiFi is used for:
- settings configuration for Rocket modules,
- recovery tests,
- settings configuration for ground station,
- ground station telemetry.

A WiFi connection can be established only in the `RAIL` state. During the flight WiFi of the rocket module is turned off to save power consumption. WiFi for both rocket and ground modules starts when the *BOOT* button on ESP32-C3 is pressed after system initalization.

The Apogemix with WiFi turned on created a network `Apogemix_<ID>`. To connect with this network you need to use `apogemix` password. After connecting, you should open the web browser and go to one of these urls to see the configuration / telemetry page:

[http://apogemix.local](http://apogemix.local)

[http://192.168.4.1](http://192.168.4.1)

For the ground station module, you do not need to turn on the WiFi for the telemetry. You can also use USB-CDC (UART) to receive telemetry on the computer and visualise it using project linked below:

[github.com/kowalDrzo/apogapka](github.com/kowalDrzo/apogapka)

### Configuration

Apogemix has settings configured in the code and settings which are designed to be configured by user using WiFi.

Configured in the code (for advanced users only):

- WiFi Tx Power set to `WIFI_POWER_8_5dBm` - for stability. Can be set higher for better range,
- `APOGEMIX_ID` - must be unique number,
- `GNSS_BAUD` set to `9600` or `115200` - depends on the used localization module,
- defined `BMP580` - use BMP580, if commented use older and less precise BMP180,
- `FLIGHTS_IN_MEM` set to `30` - how many flight information should be remembered (flight number, apogee altitude and maximum velocity). The extended flight data is available only for the last flight. I suggest not to change it,
- `FRAMES_IN_Q` set to `30` - lenght of the rotary queue. It is used to save data to flash in blocks (faster than saving each frame separately) and not to loose the begin of the flight (just before the launch detection). I suggest not to change it,
- `ALPHA_H` set to `0.2` - alpha coefficient for low pass filter for the altitude. I suggest not to change it,
- `ALPHA_V` set to `0.2` - alpha coefficient for low pass filter for the velocity. I suggest not to change it,
- `RAIL_FLIGHT_LOOP_TIME` set to `200` ms - period of frames in the `RAIL` and `FLIGHT` states. I suggest not to change it,
- `FIRST_SEPAR_LOOP_TIME` set to `250` ms - period of frames in the `FIST_SEPAR` state. I suggest not to change it,
- `SECND_SEPAR_LOOP_TIME` set to `500` ms - period of frames in the `SECOND_SEPAR` state. I suggest not to change it,
- `FIRE_TIME` set to `2000` ms - how long the mosfets should be active during chute deployment. I suggest not to change it,
- `TEMPERATURE_FIX_A` set to `0.855` - linear coefficient to fix temperature measurement. I suggest not to change it,
- `TEMPERATURE_FIX_B` set to `(-1.188)` - offset coefficient to fix temperature measurement. I suggest not to change it,
- `WIFI_TIME_MS` set to `600000` ms - turn off the WiFi after this time after enabling it,
- `CALLSIGN_LEN` set to `11` characters (10 + `\0`) - maximum lenght of the LoRa callsign. Changing it can make receiving LoRa telemetry impossible.

To configure using WiFi (for each user):

TODO

### Testing

TODO

## Author contact

The best way to contact me is just write to me using Discord: `sp3mik`.
