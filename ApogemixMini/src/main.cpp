#include <FS.h>
#include "PinDefinitions.h"
#include "Globals.h"

void setup() {

    Serial.begin(115200);

    pinMode(SEPAR1_PIN, OUTPUT);
    pinMode(SEPAR2_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(CONT1_PIN, INPUT_PULLUP);
    pinMode(CONT2_PIN, INPUT_PULLUP);

    Serial.println("siema");
    Serial.println((int)sizeof(DataFrame));
}

void loop() {

}