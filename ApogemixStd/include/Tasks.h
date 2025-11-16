#ifndef TASKS_H
#define TASKS_H

#define BMP580

#include <Arduino.h>
#include <EEPROM.h>
#include <LittleFS.h>
#include <Wire.h>
#include <SPI.h>
#ifdef BMP580
#include <Adafruit_BMP5xx.h>
#else
#include <Adafruit_BMP085.h>
#endif
#include <TinyGPS++.h>

#include "PinDefinitions.h"
#include "Timer.h"
#include "DataStructs.h"
#include "Globals.h"

class Tasks {

    uint8_t criteriaCounter = 0;
    void continuityTest();
    float getPressureMedian();

public:

    #ifdef BMP580
    Adafruit_BMP5xx
    #else
    Adafruit_BMP085
    #endif
    bmp;
    TinyGPSPlus gps;

    void buzzBeep(uint16_t activeTime, uint16_t sleepTime, uint8_t n);

    void measure();
    void buzz();

    bool isLaunchDetected();
    bool isApogeeDetected();
    bool isSecondChuteTime();
    bool isOnGround();

    static void flashTask();
    void readFlash();
    void updateDataBase();
    void recalibrate();

    void clearMem();
};

extern Tasks tasks;

#endif