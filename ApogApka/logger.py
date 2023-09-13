import time
import os

class Logger:

    fileName = "logs/log_"

    def __init__(self):
        
        os.makedirs("logs", exist_ok=True)
        self.fileName += time.strftime("%Y%m%d%H%M%S") + ".txt"

    def log(self, frameString):

        file = open(self.fileName, "a")
        file.write(frameString)
        file.close()
