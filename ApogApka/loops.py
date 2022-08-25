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
                except(IndexError):
                    decodeErrors += 1

            except(UnicodeDecodeError):
                decodeErrors += 1

    #########################################################

    def guiLoop(self):

        cap = cv2.VideoCapture(0)

        RES_X = 1280
        RES_Y = 720

        cap.set(cv2.CAP_PROP_FRAME_WIDTH, RES_X)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, RES_Y)

        fourcc = cv2.VideoWriter_fourcc(*'MP4V')
        out = cv2.VideoWriter("test.mp4", fourcc, 15, (RES_X,RES_Y))

        while True:

            if self.dataFrame:
                self.gui.showData(self.dataFrame)
            else:
                self.gui.update()

            # Capture frames in the video
            ret, frame = cap.read()

            # describe the type of font
            # to be used.
            font = cv2.FONT_HERSHEY_SIMPLEX

            # Use putText() method for
            # inserting text on video
            cv2.rotate(frame, cv2.ROTATE_180, frame)
            
            if self.dataFrame:
                cv2.putText(frame, self.dataFrame.camInfo(), (RES_X-900, RES_Y-30), font, 1.4, (255, 255, 255), 2, cv2.LINE_4)

            out.write(frame)
            # Display the resulting frame
            cv2.imshow('video', frame)

            # creating 'q' as the quit 
            # button for the video
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        # release the cap object
        cap.release()
        # close all windows
        cv2.destroyAllWindows()
        