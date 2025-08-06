import serial
import time
from data_frame import DataFrame
from gui import Gui
from logger import Logger

class Loops:

    def __init__(self):

        self.gui = Gui()
        self.logger = Logger()
        self.last_frames = {}
        self.current_callsign = "N0CALL"
        self.txQueue = []

    #########################################################

    def serialLoop(self, serialName: serial.Serial, frequencyMHz: int):

        decodeErrors = 0
        ser = serial.Serial(serialName, 115200)

        if frequencyMHz > 100:
            freqStr = "_FREQ;" + str(frequencyMHz)
            ser.write(freqStr.encode('utf-8'))

        while True:

            try:
                frameString = ser.readline().decode("utf-8")
                self.logger.log(frameString)
                vals = frameString.split(";")

                callsign = vals[0]
                dataFrame = DataFrame(vals[1:])
                self.last_frames[callsign] = dataFrame
                print("\n" + callsign)
                print(dataFrame)

                if callsign == self.current_callsign:
                    self.gui.reset_frame_delay_timer = True

                if self.txQueue:
                    txFrame = self.current_callsign + ";" + self.txQueue.pop(0) + "\n"
                    ser.write(txFrame.encode('utf-8'))
                    print("Sent: " + txFrame)

            except(UnicodeDecodeError):
                decodeErrors += 1

    #########################################################

    def guiLoop(self):

        sleeptime = 0.1

        while True:

            if self.current_callsign in self.last_frames:
                self.gui.showData(self.last_frames[self.current_callsign])
            else:
                self.gui.update()

            self.gui.updateCallsignMenu(list(self.last_frames.keys()))
            self.current_callsign = self.gui.callsignMenuVar.get()

            commandStr = self.gui.getCommand()
            if commandStr:
                self.txQueue.append(commandStr)

            time.sleep(sleeptime)
            self.gui.updateFrameDelayTimer(sleeptime)
