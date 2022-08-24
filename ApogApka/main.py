#!/usr/bin/python3

import serial
import sys
from data_frame import DataFrame
from gui import Gui

if __name__ == "__main__":

    portName = sys.argv[1]
    ser = serial.Serial(portName, 115200)

    gui = Gui()

    decodeErrors = 0

    while True:

        try:
            frameString = ser.readline().decode("utf-8")
            vals = frameString.split(";")
            try:
                dataFrame = DataFrame(vals)
                print(dataFrame)
                gui.showData(dataFrame)
            except(IndexError):
                decodeErrors += 1

        except(UnicodeDecodeError):
            decodeErrors += 1