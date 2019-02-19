import tkinter as tk
import serial, time
import serial.tools.list_ports as port_list

class Application(tk.Frame):
    ser = serial.Serial()

    def __init__(self, master=None):
        super().__init__(master)
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.hi_there = tk.Button(self)
        self.hi_there["text"] = "Show COM port"
        self.hi_there["command"] = self.show_ports
        self.hi_there.pack(side="top", padx=5, pady=5)

        self.connect = tk.Button(self)
        self.connect["text"] = "Connect COM port"
        self.connect["command"] = self.connect_port
        self.connect.pack(padx=5, pady=5)

        self.homepos = tk.Button(self)
        self.homepos["text"] = "Home"
        self.homepos["command"] = self.move_home
        self.homepos.pack(padx=5, pady=5)

        self.resetpos = tk.Button(self)
        self.resetpos["text"] = "ResetPos"
        self.resetpos["command"] = self.reset_pos
        self.resetpos.pack(padx=5, pady=5)

        self.showpos = tk.Button(self)
        self.showpos["text"] = "GetPos"
        self.showpos["command"] = self.show_pos
        self.showpos.pack(padx=5, pady=5)

        self.move = tk.Button(self)
        self.move["text"] = "Move"
        self.move["command"] = self.go_next_pos
        self.move.pack(padx=5, pady=5)

        self.E1 = tk.Entry(root, bd=1)
        self.E1.insert(tk.END,"set pos")
        self.E1.pack(padx=5, pady=5)

        self.quit = tk.Button(self, text="QUIT", command=self.master.destroy, bg='yellow')
        self.quit.pack(side="bottom", padx=5, pady=5)

    def show_ports(self):
        ports = list(port_list.comports())
        for p in ports:
            print (p)

    def connect_port(self):
        Application.ser.port = 'COM3'
        Application.ser.baudrate = 9600
        
        Application.ser.bytesize = serial.EIGHTBITS #number of bits per bytes
        
        Application.ser.parity = serial.PARITY_NONE #set parity check: no parity
        
        Application.ser.stopbits = serial.STOPBITS_ONE #number of stop bits
        Application.ser.timeout = 2              #timeout block read
        
        Application.ser.xonxoff = False     #disable software flow control
        
        Application.ser.rtscts = False     #disable hardware (RTS/CTS) flow control
        
        Application.ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
        
        Application.ser.writeTimeout = 2     #timeout for write

        try: 
            Application.ser.open()
        
        except Exception as e:
            print("error open serial port: " + str(e))
            exit()

        if Application.ser.isOpen():
            print("succeeded in connecting");
        else:
            print("cannot open serial port ")

    def move_home(self):
        Application.ser.write('F,C,I1M-0,I1M5000,I1M-0,R'.encode())

    def reset_pos(self):
        time.sleep(0.5)  #give the serial port sometime to receive the data
        Application.ser.write('N'.encode()) #Register current position as 0 

    def show_pos(self):
        Application.ser.write('F,C,X,R'.encode())
        time.sleep(0.5)  #give the serial port sometime to receive the data
        response = Application.ser.readline()
        print(" read data: " + response.decode())

    def go_next_pos(self):
        val = self.E1.get()
        command = 'F,C,I1AM' + val +',R'
        Application.ser.write(command.encode())
        print(command)

root = tk.Tk()
root.title("GUI of stepping moter")
root.geometry("400x400")
app = Application(master=root)
app.mainloop()
