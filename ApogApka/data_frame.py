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
    mosState    = False
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

        try:
            continuities     = int(vals[8])
            self.cont1       = bool(continuities & 0b001)
            self.cont2       = bool(continuities & 0b010)
            self.mosState    = bool(continuities & 0b100)
            self.rocketState = int(vals[9])
        except(ValueError):
            self.cont1       = False
            self.cont2       = False
            self.mosState    = False
            self.rocketState = 0

    #########################################################

    def __str__(self) -> str:

        frameString =  "\nGPS szerokość: " + str(self.gpsLat)
        frameString += "\nGPS długość: "   + str(self.gpsLng)
        frameString += "\nGPS wysokość: "  + str(self.gpsAlt)
        frameString += "\nCzas ms: "       + str(self.time)
        frameString += "\nTemperatura: "   + str(self.temper)
        frameString += "\nCiśnienie: "     + str(self.pressure)
        frameString += "\nWysokość: "      + str(self.altitude)
        frameString += "\nPrędkość: "      + str(self.speed)
        frameString += "\nCiągłość 1: "    + str(self.cont1)
        frameString += "\nCiągłość 2: "    + str(self.cont2)
        frameString += "\nMosfet: "        + str(self.mosState)
        frameString += "\nStan: "          + self.stateString()

        return frameString

    #########################################################

    def stateString(self) -> str:

        stateString = ""
        if self.rocketState == 0:
            stateString = "Rail"
        elif self.rocketState == 1:
            stateString = "Flight"
        elif self.rocketState == 2:
            stateString = "Apogee"
        elif self.rocketState == 3:
            stateString = "Dual Deploy"
        elif self.rocketState == 4:
            stateString = "Ground"

        return stateString

    #########################################################

    def camInfo(self) -> str:
        return "A: " + self.altitude +  "m,  S: " + self.speed + "m/s,   st: " + str(self.rocketState)