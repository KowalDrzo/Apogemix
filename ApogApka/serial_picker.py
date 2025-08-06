import serial.tools.list_ports
import tkinter as tk
from tkinter import ttk

class SerialPicker:

    frequencyMHz = 0

    def getSelectedItem(self) -> str:

        self.selected = str(self.dropdownVar.get()).split("-")[0].strip()
        if self.frequencyEntryVar.get():
            self.frequencyMHz = int(self.frequencyEntryVar.get())

        self.root.destroy()
        return self.selected

    #########################################################

    def refreshPorts(self):
        ports = []
        portObjects = serial.tools.list_ports.comports()
        for port in portObjects:
            ports.append(port.device)
        self.dropdown["values"] = ports

        if self.dropdown.get() == "" and len(ports) > 0:
            self.dropdown.set(port[0])

    #########################################################

    def pick(self):

        self.root = tk.Tk()
        self.root.title("Pick Serial")

        self.dropdownVar = tk.StringVar()
        self.dropdown = ttk.Combobox(self.root, textvariable=self.dropdownVar)
        self.dropdown.pack(padx=20, pady=10)

        self.refreshPorts()

        self.freqLabel = tk.Label(self.root, text="Frequency [MHz]")
        self.freqLabel.pack()

        self.frequencyEntryVar = tk.StringVar()
        self.frequencyEntry = tk.Entry(self.root, textvariable=self.frequencyEntryVar)
        self.frequencyEntry.pack()

        buttonFrame = tk.Frame(self.root)
        buttonFrame.pack()

        okButton = tk.Button(buttonFrame, text="OK", command=self.getSelectedItem)
        okButton.pack(side=tk.LEFT, padx=5)

        refreshButton = tk.Button(buttonFrame, text="Refresh", command=self.refreshPorts)
        refreshButton.pack(side=tk.LEFT, padx=5)

        buttonFrame.pack(pady=10)

        self.root.mainloop()
        return self.selected