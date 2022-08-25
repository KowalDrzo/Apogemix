#!/usr/bin/python3

import sys
import serial
import threading
from loops import Loops


if __name__ == "__main__":

    ser = serial.Serial(sys.argv[1], 115200)

    loops = Loops()

    serialThread = threading.Thread(target=loops.serialLoop, args=(ser,))
    serialThread.start()

    if len(sys.argv) > 2:
        camThread = threading.Thread(target=loops.camLoop, args=(sys.argv[2],))
        camThread.start()

    loops.guiLoop()