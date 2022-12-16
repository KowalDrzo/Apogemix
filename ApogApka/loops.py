import serial
import cv2
import time
from data_frame import DataFrame
from gui import Gui
from logger import Logger

class Loops:

    def __init__(self):

        self.gui = Gui()
        self.logger = Logger()
        self.dataFrame = None
        self.txQueue = []

    #########################################################

    def serialLoop(self, serialName: serial.Serial, callsign: str):

        decodeErrors = 0
        ser = serial.Serial(serialName, 115200)

        while True:

            try:
                frameString = ser.readline().decode("utf-8")
                self.logger.log(frameString)
                vals = frameString.split(";")

                try:
                    if vals[0] == callsign:
                        self.dataFrame = DataFrame(vals[1:])
                        print(self.dataFrame)
                except(IndexError):
                    decodeErrors += 1

                if self.txQueue:
                    txFrame = callsign + ";" + self.txQueue.pop(0) + "\n"
                    ser.write(txFrame.encode('utf-8'))

            except(UnicodeDecodeError):
                decodeErrors += 1

    #########################################################

    def guiLoop(self):

        while True:

            if self.dataFrame:
                self.gui.showData(self.dataFrame)
            else:
                self.gui.update()

            commandStr = self.gui.getCommand()
            if commandStr:
                self.txQueue.append(commandStr)

    #########################################################

    def camLoop(self, camPort):

        cap = cv2.VideoCapture(int(camPort[-1]))

        RES_X = 640
        RES_Y = 480

        cap.set(cv2.CAP_PROP_FRAME_WIDTH, RES_X)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, RES_Y)

        cv2.namedWindow("FPV_Cam", cv2.WINDOW_KEEPRATIO)

        videoName = "videos/vid_" + time.strftime("%Y%m%d%H%M%S") + ".mp4"
        fourcc = cv2.VideoWriter_fourcc(*'MP4V')
        out = cv2.VideoWriter(videoName, fourcc, 30, (RES_X,RES_Y))

        while True:

            ret, frame = cap.read()
            font = cv2.FONT_HERSHEY_SIMPLEX
            # For video cam inversion 180 degree:
            # cv2.rotate(frame, cv2.ROTATE_180, frame)

            if self.dataFrame:
                cv2.putText(frame, self.dataFrame.camInfo(), (10, RES_Y-30), font, 0.88, (128, 255, 255), 2, cv2.LINE_4)

            out.write(frame)
            cv2.imshow("FPV_Cam", frame)

            cv2.waitKey(1)
            if cv2.getWindowProperty("FPV_Cam", cv2.WND_PROP_VISIBLE) < 1:
                break

        cap.release()
        cv2.destroyAllWindows()