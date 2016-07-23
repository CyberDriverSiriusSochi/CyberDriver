import socket
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
i = 0

while 1:
        try:
                sock.sendto("RaspberryCyberDriver" + str(i), ("255.255.255.255", 4997))
                #print 'Sent'
                i += 1
                time.sleep(0.1)
        except Exception:
                pass
