import socket
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

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) 
while 1:
	data = raw_input("Please, write speed for both motors\n")
	sock.sendto(data, (UDP_IP, UDP_PORT))
        
