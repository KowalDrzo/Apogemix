#!/usr/bin/python3

import sys
import serial
import threading
from loops import Loops
from serial_picker import SerialPicker

if __name__ == "__main__":

    if len(sys.argv) > 1:
        serialName = sys.argv[1]
    else:
        serialPicker = SerialPicker()
        serialName = serialPicker.pick()
        frequencyMHz = serialPicker.frequencyMHz

    print("Opening Serial: " + serialName)
    loops = Loops()

    serialThread = threading.Thread(target=loops.serialLoop, args=(serialName, frequencyMHz))
    serialThread.start()

    loops.guiLoop()
