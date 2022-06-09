#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <FS.h>
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

public:

    Adafruit_BMP085 bmp;

    void measure();
    void readEeprom();

    bool isLaunchDetected();
    bool isApogeeDetected();
    bool isSecondChuteTime();
    bool isOnGround();

    void writeToFlash(bool force = false);
    void updateDataBase();

    void clearMem();
};

extern Tasks tasks;

#endif