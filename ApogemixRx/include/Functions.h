#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Globals.h>

void clearMem();

void checkForNewFrequency(String command);

void saveAndSetNewFrequency();

void addFrameToMap(String frameString);
String getFrameFromMap(String deviceName);
String getDevicesFromMap();

#endif
