#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#define APOGEMIX_ID 1
#define FLIGHTS_IN_MEM 30
#define FRAMES_IN_Q 30
#define ALPHA_H 0.2
#define ALPHA_V 0.2
#define RAIL_FLIGHT_LOOP_TIME 200
#define FIRST_SEPAR_LOOP_TIME 250
#define SECND_SEPAR_LOOP_TIME 500
#define FIRE_TIME 2000
#define TEMPERATURE_FIX_A 0.855
#define TEMPERATURE_FIX_B (-1.188)
#define WIFI_TIME_MS 600000

#include <stdint.h>

struct Flight {

    uint16_t num;
    uint16_t apogee;
    uint16_t maxSpeed;
};

struct Memory {

    uint8_t lastFlightIndex;
    uint16_t lastFlightNum;
    Flight flight[FLIGHTS_IN_MEM];

    uint16_t firstSeparDelay_ms;
    uint16_t secondSeparAltitude;
    uint16_t wifiActiveTime_min;
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
    float temper;
    float pressure;
    float altitude;
    float speed;
    bool continuity1 : 1;
    bool continuity2 : 1;
    char rocketState : 3;

    String toString() {

        // Make one byte from continuities:
        uint8_t continuities = 0;
        continuities |= (continuity1 << 0) | (continuity2 << 1);

        char data[40];
        sprintf(data, "%d;%0.1f;%0.1f;%0.1f;%0.1f;%d;%d", time, temper, pressure, altitude, speed, continuities, (int)rocketState);
        return String(data);
    }
};

#endif
