#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <LITTLEFS.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>
#include <TinyGPS++.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "PinDefinitions.h"
#include "Timer.h"
#include "DataStructs.h"
#include "Globals.h"

class Tasks {

    uint8_t criteriaCounter = 0;
    void continuityTest();
    void buzzBeep(uint16_t time, uint8_t n);

public:

    OneWire oneWire = OneWire(ONE_WIRE_PIN);
    DallasTemperature ds18b20 = DallasTemperature(&oneWire);
    Adafruit_BMP085 bmp;
    TinyGPSPlus gps;

    void measure();
    void buzz();
    float readDsTemperature();

    bool isLaunchDetected();
    bool isApogeeDetected();
    bool isSecondChuteTime();
    bool isOnGround();

    static void flashTask();
    void readFlash();
    void updateDataBase();

    void clearMem();
};

extern Tasks tasks;

#endif