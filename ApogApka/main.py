#!/usr/bin/python3

import sys
import serial
import threading
from loops import Loops

if __name__ == "__main__":

    serialName = sys.argv[1]
    callsign = sys.argv[2]

    ser = serial.Serial(sys.argv[1], 115200, timeout=0.1)

    loops = Loops()

    serialThread = threading.Thread(target=loops.serialLoop, args=(ser, callsign))
    serialThread.start()

    if len(sys.argv) > 3:
        camPort = sys.argv[3]
        camThread = threading.Thread(target=loops.camLoop, args=(camPort,))
        camThread.start()

    loops.guiLoop()