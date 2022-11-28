#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <SPI.h>
#include <TinyGPS++.h>

#include "Timer.h"
#include "DataStructs.h"

class Tasks {

public:

    TinyGPSPlus gps;
};

extern Tasks tasks;

#endif