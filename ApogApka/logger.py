import time

class Logger:

    fileName = "logs/log_"

    def __init__(self):
        
        self.fileName += time.strftime("%Y%m%d%H%M%S") + ".txt"

    def log(self, frameString):

        file = open(self.fileName, "a")
        file.write(frameString)
        file.close()