#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#define APOGEMIX_ID 7
#define FLIGHTS_IN_MEM 30
#define FRAMES_IN_Q 30
#define ALPHA_H 0.2
#define ALPHA_V 0.2
#define RAIL_FLIGHT_LOOP_TIME 200
#define FIRST_SEPAR_LOOP_TIME 500
#define SECND_SEPAR_LOOP_TIME 1000
#define FIRE_TIME 2000
#define TEMPERATURE_FIX_A 1//0.855
#define TEMPERATURE_FIX_B 0//(-1.188)

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
    uint16_t loraFreqMHz;
    uint16_t loraDelay_ms;

    char callsign[11];
};

enum RocketState {

    RAIL = 0,
    FLIGHT,
    FIRST_SEPAR,
    SECOND_SEPAR,
    GROUND
};

struct DataFrame {

    float gpsLat;
    float gpsLng;
    float gpsAlt;

    uint32_t time;
    float temper;
    float pressure;
    float altitude;
    float speed;
    bool continuity1 : 1;
    bool continuity2 : 1;
    char rocketState : 3;

    String toString() {

        char data[60];
        sprintf(data, "%0.4f;%0.4f;%0.1f;%d;%0.1f;%0.1f;%0.1f;%0.1f;%d;%d;%d", gpsLat, gpsLng, gpsAlt, time, temper, pressure, altitude, speed, continuity1, continuity2, (int)rocketState);
        return String(data);
    }
};

#endif