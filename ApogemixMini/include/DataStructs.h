#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#define FLIGHTS_IN_MEM 50
#define FRAMES_IN_Q 100

#include <stdint.h>

struct Flight {
    
    uint16_t num;
    uint16_t apogee;
    uint16_t maxSpeed;
};

struct Memory {

    uint8_t lastFlightIndex;
    Flight flight[FLIGHTS_IN_MEM];

    uint16_t firstSeparDelay_ms;
    uint16_t secondSeparAltitude;
    uint16_t wifiActiveTime;
};

enum RocketState {

    RAIL = 0,
    FLIGHT,
    FIRST_SEPAR,
    SECOND_SEPAR,
    GROUND
};

struct DataFrame {

    uint32_t time;
    float pressure;
    int16_t altitude;
    int16_t speed;
    bool continuity1 : 1;
    bool continuity2 : 1;
    char rocketState : 3;
};

#endif