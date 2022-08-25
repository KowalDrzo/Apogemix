import serial
import cv2
from data_frame import DataFrame
from gui import Gui
from logger import Logger

class Loops:

    def __init__(self):

        self.gui = Gui()
        self.logger = Logger()
        self.dataFrame = None

    #########################################################

    def serialLoop(self, serial: serial.Serial):

        decodeErrors = 0

        while True:

            try:
                frameString = serial.readline().decode("utf-8")
                self.logger.log(frameString)
                vals = frameString.split(";")

                try:
                    self.dataFrame = DataFrame(vals)
                    print(self.dataFrame)
                except(IndexError or ValueError):
                    decodeErrors += 1

            except(UnicodeDecodeError):
                decodeErrors += 1

    #########################################################

    def guiLoop(self):

        while True:

            if self.dataFrame:
                self.gui.showData(self.dataFrame)
            else:
                self.gui.update()

    #########################################################

    def camLoop(self, camPort):

        cap = cv2.VideoCapture(int(camPort[-1]))

        RES_X = 640
        RES_Y = 480

        cap.set(cv2.CAP_PROP_FRAME_WIDTH, RES_X)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, RES_Y)

        fourcc = cv2.VideoWriter_fourcc(*'MP4V')
        out = cv2.VideoWriter("test.mp4", fourcc, 30, (RES_X,RES_Y))

        while True:

            ret, frame = cap.read()
            font = cv2.FONT_HERSHEY_SIMPLEX
            cv2.rotate(frame, cv2.ROTATE_180, frame)

            if self.dataFrame:
                cv2.putText(frame, self.dataFrame.camInfo(), (10, RES_Y-30), font, 1, (255, 255, 255), 2, cv2.LINE_4)

            out.write(frame)
            cv2.imshow('video', frame)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        cap.release()
        cv2.destroyAllWindows()