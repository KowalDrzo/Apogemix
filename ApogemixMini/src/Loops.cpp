#include "Loops.h"

void StateLoops::railLoop() {

    volatile int cont1, cont2;

    while (1) {

        cont1 = digitalRead(CONT1_PIN);
        cont2 = digitalRead(CONT2_PIN);

        Serial.println(cont1);
        Serial.println(cont2);
        Serial.println("------------------");
        delay(1000);
    }
}

/*********************************************************************/

void StateLoops::flightLoop() {

}

/*********************************************************************/

void StateLoops::sep1Loop() {

}

/*********************************************************************/

void StateLoops::sep2Loop() {

}

/*********************************************************************/

void StateLoops::groundLoop() {

}