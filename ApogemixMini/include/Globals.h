#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#include "FixedFifo.h"
#include "Loops.h"

using std::vector;

struct Flight {
    
    uint16_t num;
    uint16_t apogee;
    uint16_t maxSpeed;
};

#define FLIGHTS_IN_MEM 50
#define FRAMES_IN_Q 150

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
    float altitude;
    float speed;
    bool continuity1 : 1;
    bool continuity2 : 1;
    char rocketState : 3;
};

struct GlobalAggregator {

    DataFrame dataFrame;
    FixedFifo<DataFrame, FRAMES_IN_Q> dataFramesFifo;
    Memory memory;
    float initialPressure;
    uint8_t rocketState : 3;
    StateLoops loops;
};

extern GlobalAggregator glob;

#endif