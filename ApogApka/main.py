#!/usr/bin/python3

from time import sleep
import serial
import sys
from data_frame import DataFrame

portName = sys.argv[1]
ser = serial.Serial(portName, 115200)

decodeErrors = 0

while True:

    try:
        frameString = ser.readline().decode("utf-8")
        vals = frameString.split(";")
        try:
            dataFrame = DataFrame(vals)
            print(dataFrame)
        except(IndexError):
            decodeErrors += 1

    except(UnicodeDecodeError):
        decodeErrors += 1