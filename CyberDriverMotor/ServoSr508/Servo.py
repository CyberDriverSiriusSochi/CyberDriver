import serial
import re

data = ""

ttyi = 0
while ttyi < 10:
    try:
        ser = serial.Serial("/dev/ttyUSB" + str(ttyi), 9600)
        print("/dev/ttyUSB" + str(ttyi))
        while 1:

	    data = ser.readline()
  	    print data
    except serial.SerialException:
        ttyi += 1
print("No servo")
