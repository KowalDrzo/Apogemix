import tkinter as tk
import tkinter.font as tkFont
import os
from data_frame import DataFrame

class Gui:

    def __init__(self):

        self.root = tk.Tk()
        #setting title
        self.root.title("ApogApka")
        #setting window size
        width=800
        height=600
        screenwidth = self.root.winfo_screenwidth()
        screenheight = self.root.winfo_screenheight()
        alignstr = '%dx%d+%d+%d' % (width, height, (screenwidth - width) / 2, (screenheight - height) / 2)
        self.root.geometry(alignstr)
        self.root.resizable(True, True)

        gpsLatLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        gpsLatLabel["font"] = ft
        gpsLatLabel["fg"] = "#333333"
        gpsLatLabel["justify"] = "center"
        gpsLatLabel["text"] = "GPS szerokość"
        gpsLatLabel.place(x=20,y=60,width=180,height=40)

        gpsLngLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        gpsLngLabel["font"] = ft
        gpsLngLabel["fg"] = "#333333"
        gpsLngLabel["justify"] = "center"
        gpsLngLabel["text"] = "GPS długość"
        gpsLngLabel.place(x=310,y=60,width=180,height=40)

        gpsAltLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        gpsAltLabel["font"] = ft
        gpsAltLabel["fg"] = "#333333"
        gpsAltLabel["justify"] = "center"
        gpsAltLabel["text"] = "GPS wysokość"
        gpsAltLabel.place(x=580,y=60,width=180,height=40)

        timeLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        timeLabel["font"] = ft
        timeLabel["fg"] = "#333333"
        timeLabel["justify"] = "center"
        timeLabel["text"] = "Czas ms"
        timeLabel.place(x=310,y=340,width=180,height=40)

        temperatureLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        temperatureLabel["font"] = ft
        temperatureLabel["fg"] = "#333333"
        temperatureLabel["justify"] = "center"
        temperatureLabel["text"] = "Temperatura"
        temperatureLabel.place(x=20,y=340,width=180,height=40)

        pressureLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        pressureLabel["font"] = ft
        pressureLabel["fg"] = "#333333"
        pressureLabel["justify"] = "center"
        pressureLabel["text"] = "Ciśnienie"
        pressureLabel.place(x=20,y=200,width=180,height=40)

        altitudeLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        altitudeLabel["font"] = ft
        altitudeLabel["fg"] = "#333333"
        altitudeLabel["justify"] = "center"
        altitudeLabel["text"] = "Wysokość"
        altitudeLabel.place(x=310,y=200,width=180,height=40)

        speedLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        speedLabel["font"] = ft
        speedLabel["fg"] = "#333333"
        speedLabel["justify"] = "center"
        speedLabel["text"] = "Prędkość"
        speedLabel.place(x=580,y=200,width=180,height=40)

        cont1Label=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        cont1Label["font"] = ft
        cont1Label["fg"] = "#333333"
        cont1Label["justify"] = "center"
        cont1Label["text"] = "Ciągłość 1"
        cont1Label.place(x=560,y=340,width=100,height=40)

        cont2Label=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        cont2Label["font"] = ft
        cont2Label["fg"] = "#333333"
        cont2Label["justify"] = "center"
        cont2Label["text"] = "Ciągłość 2"
        cont2Label.place(x=560,y=400,width=100,height=40)

        stateLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        stateLabel["font"] = ft
        stateLabel["fg"] = "#333333"
        stateLabel["justify"] = "center"
        stateLabel["text"] = "Stan rakiety"
        stateLabel.place(x=560,y=460,width=100,height=40)

        self.test1Button=tk.Button(self.root)
        self.test1Button["bg"] = "#e9e9ed"
        ft = tkFont.Font(family='Arial',size=13)
        self.test1Button["font"] = ft
        self.test1Button["fg"] = "#000000"
        self.test1Button["justify"] = "center"
        self.test1Button["text"] = "Test 1"
        self.test1Button.place(x=200,y=510,width=70,height=30)
        self.test1Button["command"] = self.test1Button_command
        self.test1Button["state"] = tk.DISABLED

        self.test2Button=tk.Button(self.root)
        self.test2Button["bg"] = "#e9e9ed"
        ft = tkFont.Font(family='Arial',size=13)
        self.test2Button["font"] = ft
        self.test2Button["fg"] = "#000000"
        self.test2Button["justify"] = "center"
        self.test2Button["text"] = "Test 2"
        self.test2Button.place(x=200,y=550,width=70,height=30)
        self.test2Button["command"] = self.test2Button_command
        self.test2Button["state"] = tk.DISABLED

        self.test1Active=tk.Checkbutton(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        self.test1Active["font"] = ft
        self.test1Active["fg"] = "#333333"
        self.test1Active["justify"] = "center"
        self.test1Active["text"] = "Aktywuj Test 1"
        self.test1Active.place(x=50,y=510,width=150,height=25)
        self.test1Active["offvalue"] = "0"
        self.test1Active["onvalue"] = "1"
        self.test1Active["command"] = self.test1Active_command

        self.test2Active=tk.Checkbutton(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        self.test2Active["font"] = ft
        self.test2Active["fg"] = "#333333"
        self.test2Active["justify"] = "center"
        self.test2Active["text"] = "Aktywuj Test 2"
        self.test2Active.place(x=50,y=550,width=150,height=25)
        self.test2Active["offvalue"] = "0"
        self.test2Active["onvalue"] = "1"
        self.test2Active["command"] = self.test2Active_command

        self.gpsLatVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.gpsLatVal["font"] = ft
        self.gpsLatVal["fg"] = "#333333"
        self.gpsLatVal["justify"] = "center"
        self.gpsLatVal["text"] = "+NN.NNNN"
        self.gpsLatVal.place(x=0,y=100,width=260,height=80)

        self.gpsLngVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.gpsLngVal["font"] = ft
        self.gpsLngVal["fg"] = "#333333"
        self.gpsLngVal["justify"] = "center"
        self.gpsLngVal["text"] = "+EEE.EEEE"
        self.gpsLngVal.place(x=270,y=100,width=260,height=80)

        self.gpsAltVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.gpsAltVal["font"] = ft
        self.gpsAltVal["fg"] = "#333333"
        self.gpsAltVal["justify"] = "center"
        self.gpsAltVal["text"] = "AAAAA.A"
        self.gpsAltVal.place(x=530,y=100,width=260,height=80)

        self.pressureVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.pressureVal["font"] = ft
        self.pressureVal["fg"] = "#333333"
        self.pressureVal["justify"] = "center"
        self.pressureVal["text"] = "PPPPPP.P"
        self.pressureVal.place(x=0,y=240,width=260,height=80)

        self.altitudeVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.altitudeVal["font"] = ft
        self.altitudeVal["fg"] = "#333333"
        self.altitudeVal["justify"] = "center"
        self.altitudeVal["text"] = "AAAAA.A"
        self.altitudeVal.place(x=280,y=240,width=260,height=80)

        self.speedVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.speedVal["font"] = ft
        self.speedVal["fg"] = "#333333"
        self.speedVal["justify"] = "center"
        self.speedVal["text"] = "SSSS.S"
        self.speedVal.place(x=530,y=240,width=260,height=80)

        self.temperatureVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.temperatureVal["font"] = ft
        self.temperatureVal["fg"] = "#333333"
        self.temperatureVal["justify"] = "center"
        self.temperatureVal["text"] = "+TT.T"
        self.temperatureVal.place(x=0,y=380,width=260,height=80)

        self.timeVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.timeVal["font"] = ft
        self.timeVal["fg"] = "#333333"
        self.timeVal["justify"] = "center"
        self.timeVal["text"] = "TTTTTTTT"
        self.timeVal.place(x=280,y=380,width=260,height=80)

        self.cont1Val=tk.Checkbutton(self.root)
        ft = tkFont.Font(family='Arial',size=10)
        self.cont1Val["font"] = ft
        self.cont1Val["fg"] = "#333333"
        self.cont1Val["justify"] = "center"
        self.cont1Val["text"] = ""
        self.cont1Val.place(x=670,y=340,width=40,height=40)
        self.cont1Val["offvalue"] = "0"
        self.cont1Val["onvalue"] = "1"
        self.cont1Val["command"] = self.cont1Val_command
        self.cont1Val["state"] = tk.DISABLED

        self.cont2Val=tk.Checkbutton(self.root)
        ft = tkFont.Font(family='Arial',size=10)
        self.cont2Val["font"] = ft
        self.cont2Val["fg"] = "#333333"
        self.cont2Val["justify"] = "center"
        self.cont2Val["text"] = ""
        self.cont2Val.place(x=670,y=400,width=40,height=40)
        self.cont2Val["offvalue"] = "0"
        self.cont2Val["onvalue"] = "1"
        self.cont2Val["command"] = self.cont2Val_command
        self.cont2Val["state"] = tk.DISABLED

        self.stateVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        self.stateVal["font"] = ft
        self.stateVal["fg"] = "#333333"
        self.stateVal["justify"] = "center"
        self.stateVal["text"] = "Text"
        self.stateVal.place(x=660,y=460,width=130,height=40)

        self.txQueue = []
        self.root.protocol("WM_DELETE_WINDOW", self.onClosing)

    #########################################################

    def update(self):

        self.root.update()

    #########################################################

    def onClosing(self):

        self.root.destroy()
        os._exit(0)

    #########################################################

    def showData(self, dataFrame: DataFrame):

        self.gpsLatVal["text"] = dataFrame.gpsLat
        self.gpsLngVal["text"] = dataFrame.gpsLng
        self.gpsAltVal["text"] = dataFrame.gpsAlt
        self.pressureVal["text"] = dataFrame.pressure
        self.altitudeVal["text"] = dataFrame.altitude
        self.speedVal["text"] = dataFrame.speed
        self.temperatureVal["text"] = dataFrame.temper
        self.timeVal["text"] = dataFrame.time
        self.stateVal["text"] = dataFrame.stateString()

        if dataFrame.cont1:
            self.cont1Val.select()
        else:
            self.cont1Val.deselect()

        if dataFrame.cont2:
            self.cont2Val.select()
        else:
            self.cont2Val.deselect()

        self.update()

    #########################################################

    def test1Button_command(self):
        self.txQueue.append("TEST1")

    #########################################################

    def test2Button_command(self):
        self.txQueue.append("TEST2")

    #########################################################

    def test1Active_command(self):

        if self.test1Button["state"] == tk.DISABLED:
            self.test1Button["state"] = tk.NORMAL
        else:
            self.test1Button["state"] = tk.DISABLED

    #########################################################

    def test2Active_command(self):

        if self.test2Button["state"] == tk.DISABLED:
            self.test2Button["state"] = tk.NORMAL
        else:
            self.test2Button["state"] = tk.DISABLED

    #########################################################

    def cont1Val_command(self):
        print("command")

    #########################################################

    def cont2Val_command(self):
        print("command")

    #########################################################

    def getCommand(self) -> str:

        retStr = ""
        if self.txQueue:
            retStr = self.txQueue.pop(0)
            return retStr