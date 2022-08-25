#!/usr/bin/python3

import sys
import serial
import threading
from loops import Loops


if __name__ == "__main__":

    portName = sys.argv[1]
    ser = serial.Serial(portName, 115200)

    loops = Loops()

    serialThread = threading.Thread(target=loops.serialLoop, args=(ser,))
    serialThread.start()
    loops.guiLoop()