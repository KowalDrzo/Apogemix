#include "Functions.h"

void prepareMem() {

    constexpr uint16_t INITIALIZATION_MAGIC = 0x2137;
    if (glob.memory.initializationMagic == INITIALIZATION_MAGIC) {
        if (glob.memory.frequencyMHz >= MIN_FREQUENCY_MHZ &&
            glob.memory.frequencyMHz <= MAX_FREQUENCY_MHZ) return;

        glob.memory.frequencyMHz = 433.0f;
        EEPROM.put(0, glob.memory);
        EEPROM.commit();
        return;
    }

    glob.memory.frequencyMHz = 433.0f;
    glob.memory.initializationMagic = INITIALIZATION_MAGIC;

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
}

/*********************************************************************/

void checkForNewFrequency(String command) {

    const char* frequencyCommand = strstr(command.c_str(), "_FREQ;");
    if (frequencyCommand) {

        float newFreq;
        if (sscanf(frequencyCommand, "_FREQ;%f", &newFreq) != 1 ||
            !(newFreq >= MIN_FREQUENCY_MHZ && newFreq <= MAX_FREQUENCY_MHZ)) return;

        updateFrequency(newFreq);
    }
}

/*********************************************************************/

bool updateFrequency(float frequencyMHz) {

    if (!(frequencyMHz >= MIN_FREQUENCY_MHZ && frequencyMHz <= MAX_FREQUENCY_MHZ)) return false;

    glob.memory.frequencyMHz = frequencyMHz;

    EEPROM.put(0, glob.memory);
    EEPROM.commit();

    glob.loraModule.setFrequency(glob.memory.frequencyMHz);

    digitalWrite(LED_PIN, 1);
    vTaskDelay(30 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, 0);
    vTaskDelay(30 / portTICK_PERIOD_MS);
    return true;
}
