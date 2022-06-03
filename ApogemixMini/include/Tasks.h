#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>

#include "PinDefinitions.h"
#include "Timer.h"
#include "DataStructs.h"
#include "Globals.h"

class Tasks {

public:

    Adafruit_BMP085 bmp;

    void measure();
    void readEeprom();
};

extern Tasks tasks;

#endif