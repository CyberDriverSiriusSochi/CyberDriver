import serial
import socket
import re
import time

class PID:
    Isum = 0.0
    LastInput = 0.0
    P = None
    I = None
    D = None
    value = 0.0
    def __init__(self, _P, _I, _D):
        self.P = _P
        self.I = _I
        self.D = _D

    def pid(self, target):
        self.Isum += target - self.value
        self.result = self.P * (target - self.value) + self.I * self.Isum + self.D * (self.value - self.LastInput)
        self.LastInput = self.value
        self.value += self.result
        return self.value
        
#UDP_IP = "10.23.47.207"
#Broadcast

UDP_IP = ""
UDP_PORT = 4999

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

#We must send realtime data to arduino
sock.settimeout(0.1)

pidL = PID(0.2, 0.0, 0.4)
pidR = PID(0.2, 0.0, 0.4)
value1 = 0
value2 = 0
data = ""

while 1:
    ttyi = 0
    while ttyi < 10:
        try:
            ser = serial.Serial("/dev/ttyACM" + str(ttyi), 9600)
            print("/dev/ttyACM" + str(ttyi))
            while 1:
                try:
                    data, addr = sock.recvfrom(32)
                    #String for first number
                    value1S = ""
                    #String for second number
                    value2S = ""
                    #Array of numbers
                    Numbers = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '.']
                    #Parse first number
                    startIndex = 0
                    for x in data:
                        if x in Numbers:
                            value1S += x
                            startIndex += 1
                        else:
                            break;
                        
                    #Search start of second number
                    for x in range(startIndex, len(data)):
                        if data[x] in Numbers:
                            break;
                        startIndex += 1
                
                    #Parse second number
                    for x in range(startIndex, len(data)):
                        if data[x] in Numbers:
                            value2S += data[x]
                        else:   
                            break;
                        
                    value1 = int(value1S)
                    value2 = int(value2S)
                
                    #value
                except Exception:
                    pass
    
                value1Send = round(pidL.pid(value1))
                value2Send = round(pidR.pid(value2))
                value1Send = int(value1Send)
                value2Send = int(value2Send)
                print "message:", str(value1Send) + " " + str(value2Send)
                ser.write(str(value1Send) + " " + str(value2Send) + "\n" )

    #            data = ser.readline()
    #            print "Sonic: ", data
  
        except serial.SerialException:
            ttyi += 1
    print("No Arduino")
    time.sleep(0.1)
