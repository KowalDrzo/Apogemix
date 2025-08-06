import tkinter as tk
from tkinter import ttk
import tkinter.font as tkFont
import time
import os
from data_frame import DataFrame

class Gui:

    commands_list = ["RECALIBRATE", "MOS_ON", "MOS_OFF", "MOS_CLK"]
    reset_frame_delay_timer = False

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

        self.forceExit = False

        callsignMenuLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        callsignMenuLabel["font"] = ft
        callsignMenuLabel["fg"] = "#333333"
        callsignMenuLabel["justify"] = "center"
        callsignMenuLabel["text"] = "Callsign:"
        callsignMenuLabel.place(x=250,y=20)

        self.callsignMenuVar = tk.StringVar()
        self.callsignMenu = ttk.Combobox(self.root, textvariable=self.callsignMenuVar)
        self.callsignMenu.place(x=400, y=20)

        gpsStrLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        gpsStrLabel["font"] = ft
        gpsStrLabel["fg"] = "#333333"
        gpsStrLabel["justify"] = "center"
        gpsStrLabel["text"] = "GPS location"
        gpsStrLabel.place(x=310,y=60,width=180,height=40)

        timeLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        timeLabel["font"] = ft
        timeLabel["fg"] = "#333333"
        timeLabel["justify"] = "center"
        timeLabel["text"] = "Last frame delay"
        timeLabel.place(x=160,y=340,width=230,height=40)

        altitudeLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=18)
        altitudeLabel["font"] = ft
        altitudeLabel["fg"] = "#333333"
        altitudeLabel["justify"] = "center"
        altitudeLabel["text"] = "Altitude [m]"
        altitudeLabel.place(x=310,y=200,width=180,height=40)

        cont1Label=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        cont1Label["font"] = ft
        cont1Label["fg"] = "#333333"
        cont1Label["justify"] = "center"
        cont1Label["text"] = "Cont. 1"
        cont1Label.place(x=560,y=340,width=100,height=40)

        cont2Label=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        cont2Label["font"] = ft
        cont2Label["fg"] = "#333333"
        cont2Label["justify"] = "center"
        cont2Label["text"] = "Cont. 2"
        cont2Label.place(x=560,y=390,width=100,height=40)

        MosLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        MosLabel["font"] = ft
        MosLabel["fg"] = "#333333"
        MosLabel["justify"] = "center"
        MosLabel["text"] = "Mosfet"
        MosLabel.place(x=560,y=440,width=100,height=40)

        stateLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        stateLabel["font"] = ft
        stateLabel["fg"] = "#333333"
        stateLabel["justify"] = "center"
        stateLabel["text"] = "Rocket state"
        stateLabel.place(x=500,y=490,width=180,height=40)

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
        self.test1Active["text"] = "Active Test 1"
        self.test1Active.place(x=40,y=510,width=150,height=25)
        self.test1Active["offvalue"] = "0"
        self.test1Active["onvalue"] = "1"
        self.test1Active["command"] = self.test1Active_command

        self.test2Active=tk.Checkbutton(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        self.test2Active["font"] = ft
        self.test2Active["fg"] = "#333333"
        self.test2Active["justify"] = "center"
        self.test2Active["text"] = "Active Test 2"
        self.test2Active.place(x=40,y=550,width=150,height=25)
        self.test2Active["offvalue"] = "0"
        self.test2Active["onvalue"] = "1"
        self.test2Active["command"] = self.test2Active_command

        self.gpsStrVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.gpsStrVal["font"] = ft
        self.gpsStrVal["fg"] = "#333333"
        self.gpsStrVal["justify"] = "center"
        self.gpsStrVal["text"] = "+NN.NNNN,+EEE.EEEE"
        self.gpsStrVal.place(x=0,y=100,width=780,height=80)

        self.altitudeVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.altitudeVal["font"] = ft
        self.altitudeVal["fg"] = "#333333"
        self.altitudeVal["justify"] = "center"
        self.altitudeVal["text"] = "AAAAA.A"
        self.altitudeVal.place(x=280,y=240,width=260,height=80)

        self.timeVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=38)
        self.timeVal["font"] = ft
        self.timeVal["fg"] = "#333333"
        self.timeVal["justify"] = "center"
        self.timeVal["text"] = "0.0"
        self.timeVal.place(x=140,y=380,width=300,height=80)

        self.cont1Val=tk.Checkbutton(self.root)
        ft = tkFont.Font(family='Arial',size=10)
        self.cont1Val["font"] = ft
        self.cont1Val["fg"] = "#333333"
        self.cont1Val["justify"] = "center"
        self.cont1Val["text"] = ""
        self.cont1Val.place(x=670,y=340,width=40,height=40)
        self.cont1Val["offvalue"] = "0"
        self.cont1Val["onvalue"] = "1"
        self.cont1Val["state"] = tk.DISABLED

        self.cont2Val=tk.Checkbutton(self.root)
        ft = tkFont.Font(family='Arial',size=10)
        self.cont2Val["font"] = ft
        self.cont2Val["fg"] = "#333333"
        self.cont2Val["justify"] = "center"
        self.cont2Val["text"] = ""
        self.cont2Val.place(x=670,y=390,width=40,height=40)
        self.cont2Val["offvalue"] = "0"
        self.cont2Val["onvalue"] = "1"
        self.cont2Val["state"] = tk.DISABLED

        self.mosVal=tk.Checkbutton(self.root)
        ft = tkFont.Font(family='Arial',size=10)
        self.mosVal["font"] = ft
        self.mosVal["fg"] = "#333333"
        self.mosVal["justify"] = "center"
        self.mosVal["text"] = ""
        self.mosVal.place(x=670,y=440,width=40,height=40)
        self.mosVal["offvalue"] = "0"
        self.mosVal["onvalue"] = "1"
        self.mosVal["state"] = tk.DISABLED

        self.stateVal=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        self.stateVal["font"] = ft
        self.stateVal["fg"] = "#333333"
        self.stateVal["justify"] = "center"
        self.stateVal["text"] = "Text"
        self.stateVal.place(x=660,y=490,width=130,height=40)

        self.progress = ttk.Progressbar(self.root, length=230)
        self.progress.place(x=530,y=540)

        commandsMenuLabel=tk.Label(self.root)
        ft = tkFont.Font(family='Arial',size=13)
        commandsMenuLabel["font"] = ft
        commandsMenuLabel["fg"] = "#333333"
        commandsMenuLabel["justify"] = "center"
        commandsMenuLabel["text"] = "Commands:"
        commandsMenuLabel.place(x=340,y=480)

        self.commandsMenuVar = tk.StringVar()
        self.commandsMenu = ttk.Combobox(self.root, textvariable=self.commandsMenuVar)
        self.commandsMenu.place(x=300, y=510)
        self.commandsMenu["value"] = self.commands_list
        self.commandsMenu.current(0)

        self.commandSendButton=tk.Button(self.root)
        self.commandSendButton["bg"] = "#e9e9ed"
        ft = tkFont.Font(family='Arial',size=13)
        self.commandSendButton["font"] = ft
        self.commandSendButton["fg"] = "#000000"
        self.commandSendButton["justify"] = "center"
        self.commandSendButton["text"] = "Send"
        self.commandSendButton.place(x=350,y=540,width=70,height=30)
        self.commandSendButton["command"] = self.sendButton_command

        self.txQueue = []
        self.root.protocol("WM_DELETE_WINDOW", self.onClosing)

    #########################################################

    def update(self):

        self.root.update()

    #########################################################

    def onClosing(self):

        self.root.destroy()
        self.forceExit = True
        time.sleep(0.25)
        os._exit(0)

    #########################################################

    def showData(self, dataFrame: DataFrame):

        self.gpsStrVal["text"] = f"{dataFrame.gpsLat},{dataFrame.gpsLng}"
        self.altitudeVal["text"] = dataFrame.altitude
        #self.timeVal["text"] = dataFrame.time
        self.stateVal["text"] = dataFrame.stateString()

        if dataFrame.cont1:
            self.cont1Val.select()
        else:
            self.cont1Val.deselect()

        if dataFrame.cont2:
            self.cont2Val.select()
        else:
            self.cont2Val.deselect()

        if dataFrame.mosState:
            self.mosVal.select()
        else:
            self.mosVal.deselect()

        self.progress["value"] = (dataFrame.rocketState + 1) * 20

        self.update()

    #########################################################

    def updateCallsignMenu(self, callsign_list):

        self.callsignMenu["values"] = callsign_list
        if self.callsignMenu.get() == "" and len(callsign_list) > 0:
            self.callsignMenu.set(callsign_list[0])

    #########################################################

    def test1Button_command(self):
        self.txQueue.append("TEST1")

    #########################################################

    def test2Button_command(self):
        self.txQueue.append("TEST2")

    #########################################################

    def sendButton_command(self):

        command = self.commandsMenuVar.get()
        self.txQueue.append(command)

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

    def getCommand(self) -> str:

        retStr = ""
        if self.txQueue:
            retStr = self.txQueue.pop(0)
            return retStr

    #########################################################

    def updateFrameDelayTimer(self, time_s: float):

        if self.reset_frame_delay_timer:
            self.timeVal["text"] = 0.0
            self.reset_frame_delay_timer = False

        else:
            currentFrameDelay = float(self.timeVal["text"])
            self.timeVal["text"] = round(currentFrameDelay + time_s, 1)
