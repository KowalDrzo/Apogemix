#include "Functions.h"

void prepareMem() {

    constexpr uint16_t INITIALIZATION_MAGIC = 0x2137;
    if (glob.memory.initializationMagic == INITIALIZATION_MAGIC) return;

    glob.memory.frequencyMHz = 433;
    glob.memory.initializationMagic = INITIALIZATION_MAGIC;

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
}

/*********************************************************************/

void checkForNewFrequency(String command) {

    if (strstr(command.c_str(), "_FREQ;")) {

        uint16_t newFreq;
        sscanf(command.c_str(), "_FREQ;%d", &newFreq);

        glob.memory.frequencyMHz = newFreq;
        saveAndSetNewFrequency();
    }
}

/*********************************************************************/

void saveAndSetNewFrequency() {

    EEPROM.put(0, glob.memory);
    EEPROM.commit();

    glob.loraModule.setFrequency(glob.memory.frequencyMHz);

    digitalWrite(LED_PIN, 1);
    vTaskDelay(30 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, 0);
    vTaskDelay(30 / portTICK_PERIOD_MS);
}
