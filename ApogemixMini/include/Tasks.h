#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <LittleFS.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>

#include "PinDefinitions.h"
#include "Timer.h"
#include "DataStructs.h"
#include "Globals.h"

class Tasks {

    File file;
    bool appendFlash = false;
    uint8_t criteriaCounter = 0;
    void continuityTest();
    float getPressureMedian();

public:

    Adafruit_BMP085 bmp;

    void buzzBeep(uint16_t activeTime, uint16_t sleepTime, uint8_t n);

    void measure();
    void buzz();

    bool isLaunchDetected();
    bool isApogeeDetected();
    bool isSecondChuteTime();
    bool isOnGround();

    void writeToFlash(bool force = false);
    void readFlash();
    void updateDataBase();
    void recalibrate();

    void clearMem();
};

extern Tasks tasks;

#endif