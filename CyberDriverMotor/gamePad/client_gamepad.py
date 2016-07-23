import socket
import gamepad
from math import sqrt
from time import sleep
#cs = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#cs.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#cs.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
#cs.sendto('Raspberry, where you are?', ('<broadcast>', 4999))

UDP_IP = "255.255.255.255" #"10.23.46.31"
UDP_PORT = 4999
MESSAGE = "0 0"
print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

axisX = 0
axisY = 0
left = 0
right = 0
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) 
while 1:
        MESSAGE = gamepad.get() #raw_input("Please, write speed for both motors\n")
        #sock.sendto(str(MESSAGE[0])+str(MESSAGE[1]), (UDP_IP, UDP_PORT))
        axisX = float(MESSAGE[0])
        axisY = float(MESSAGE[1])
        axisY = axisY * ( -1 )
	"""
        if  axisX <= 0 and axisY <= 0:
             left = 255 * axisY  *(-1)
             right =255 * sqrt (axisX * axisX + axisY * axisY)

        if  axisX <= 0 and axisY > 0:
             left = axisY  *(-255)
             right = sqrt (axisX * axisX + axisY * axisY) * (-255)

        if  axisX > 0 and axisY > 0:
             right = axisY  *(-255)
             left = sqrt (axisX * axisX + axisY * axisY) * (-255)

        if  axisX > 0 and axisY <= 0:
             right = axisY  *(-255)
             left = sqrt (axisX * axisX + axisY * axisY) * 255

	if right > 255.0:
		right = 255.0
	if right < -255.0:
		right = -255.0

	if left > 255.0:
		left = 255.0
	if left < -255.0:
		left = -255.0
	"""
        right = ((axisY - axisX) * 255)
        left = ((axisY + axisX) * 255)

	#if axisY < 0:
	#	if axisX < 0:
	#		left, right = right * (-1), left
	#	else:		
	#		left, right = right, left * (-1)
		

	if right > 255.0:
		right = 255.0
	if right < -255.0:
		right = -255.0

	if left > 255.0:
		left = 255.0
	if left < -255.0:
		left = -255.0

        left = int(round(left))
	right = int(round(right))
        sock.sendto(str(left)+" "+str(right), (UDP_IP, UDP_PORT))
        sleep(0.3)
