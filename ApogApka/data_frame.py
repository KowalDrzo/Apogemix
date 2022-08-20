
from os import stat
from re import U


class DataFrame:

    gpsLat      = 0.0
    gpsLng      = 0.0
    gpsAlt      = 0.0
    time        = 0
    temper      = 0.0
    pressure    = 0.0
    altitude    = 0.0
    speed       = 0.0
    cont1       = False
    cont2       = False
    rocketState = 0

    def __init__(self, vals) -> None:
        
        self.gpsLat      = vals[0]
        self.gpsLng      = vals[1]
        self.gpsAlt      = vals[2]
        self.time        = vals[3]
        self.temper      = vals[4]
        self.pressure    = vals[5]
        self.altitude    = vals[6]
        self.speed       = vals[7]
        self.cont1       = vals[8]
        self.cont2       = vals[9]
        self.rocketState = vals[10]

    def __str__(self) -> str:
        
        stateString = ""
        if self.rocketState == 0:
            stateString = "Na wyrzutni"
        elif self.rocketState == 1:
            stateString = "W locie"
        elif self.rocketState == 2:
            stateString = "Pierwszy odzysk"
        elif self.rocketState == 3:
            stateString = "Drugi odzysk"
        elif self.rocketState == 4:
            stateString = "Na ziemii"

        frameString = ""
        frameString += "\nGPS szerokość: " + str(self.gpsLat)
        frameString += "\nGPS długość: "   + str(self.gpsLng)
        frameString += "\nGPS wysokość: "  + str(self.gpsAlt)
        frameString += "\nCzas ms: "       + str(self.time)
        frameString += "\nTemperatura: "   + str(self.temper)
        frameString += "\nCiśnienie: "     + str(self.pressure)
        frameString += "\nWysokość: "      + str(self.altitude)
        frameString += "\nPrędkość: "      + str(self.speed)
        frameString += "\nCiągłość 1: "    + str(self.cont1)
        frameString += "\nCiągłość 2: "    + str(self.cont2)
        frameString += "\nStan: "          + stateString

        return frameString