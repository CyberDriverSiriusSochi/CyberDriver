import socket
import re

#Broadcast
UDP_IP = ""
UDP_PORT = 4997

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
while 1:
	try:
		data, addr = sock.recvfrom(32)
		#if data[0] == "R" and data[20] == "r":
		print data, " ", addr
	except Exception:
		print "EXCEPTION"
