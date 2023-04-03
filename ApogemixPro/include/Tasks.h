#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <LITTLEFS.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>
#include <TinyGPS++.h>
#include <Servo.h>

#include "PinDefinitions.h"
#include "Timer.h"
#include "DataStructs.h"
#include "Globals.h"

class Tasks {

    uint8_t criteriaCounter = 0;
    void continuityTest();
    Servo servos[2];

public:

    Adafruit_BMP085 bmp;
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

    void servosInit();
    void servosSet(bool isApogee);
};

extern Tasks tasks;

#endif