#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

enum State {

    RAIL = 0,
    FLIGHT,
    FIRST_SEPAR,
    SECND_SEPAR,
    GROUND
};

struct DataFrame {

    /*uint32_t time;
    float pressure;
    float altitude;
    float speed;
    bool continuity1 : 1;
    bool continuity2 : 1;*/
    char rocketState : 4;
};


#endif