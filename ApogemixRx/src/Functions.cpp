#include "Functions.h"

void clearMem() {

    glob.frequencyMHz = 433;

    EEPROM.put(0, glob.frequencyMHz);
    EEPROM.commit();

    for (uint8_t i = 0; i < 10; i++) {

        digitalWrite(LED_PIN, 1);
        vTaskDelay(30 / portTICK_PERIOD_MS);
        digitalWrite(LED_PIN, 0);
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }

    while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/

void checkForNewFrequency(String command) {

    if (strstr(command.c_str(), "_FREQ;")) {

        uint16_t newFreq;
        sscanf(command.c_str(), "_FREQ;%d", &newFreq);

        glob.frequencyMHz = newFreq;
        saveAndSetNewFrequency();
    }
}

/*********************************************************************/

void saveAndSetNewFrequency() {

    EEPROM.put(0, glob.frequencyMHz);
    EEPROM.commit();

    glob.loraModule.setFrequency(glob.frequencyMHz);

    digitalWrite(LED_PIN, 1);
    vTaskDelay(30 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, 0);
    vTaskDelay(30 / portTICK_PERIOD_MS);
}

/*********************************************************************/

void addFrameToMap(String frameString) {

    int delimiterPos = frameString.indexOf(";");

    if (delimiterPos != -1) {

        String deviceName = frameString.substring(0, delimiterPos);

        if (deviceName.length() <= 11) {
            glob.deviceDict.insert(deviceName);
        }
    }
}

/*********************************************************************/

String getDevicesFromMap() {

    String deviceNames;

    for (const auto& device : glob.deviceDict) {
        deviceNames += device + ";";
    }

    if (deviceNames.endsWith(";")) {
        deviceNames.remove(deviceNames.length() - 1);
    }

    return deviceNames;
}
