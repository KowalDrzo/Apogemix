#include "Tasks.h"

Tasks tasks;

void Tasks::measure() {

    glob.dataFrame.pressure = bmp.readPressure();
    glob.dataFrame.altitude = bmp.readAltitude(glob.initialPressure);
    
    glob.dataFrame.continuity1 = !digitalRead(CONT1_PIN);
    glob.dataFrame.continuity2 = !digitalRead(CONT2_PIN);

    glob.dataFrame.time = millis();
}