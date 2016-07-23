import cv
import socket

TCP_IP = '127.0.0.2'
TCP_PORT = 4993
BUFFER_SIZE = 1024*1024*20
MESSAGE = "Hello, World!"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    cv.NamedWindow("capture", cv.CV_WINDOW_AUTOSIZE)
    s.connect((TCP_IP, TCP_PORT))
    #s.send(MESSAGE)
    data = s.recv(BUFFER_SIZE)
#    cv.ShowImage("capture", data)
#    cv.WaitKey()
    s.close()

    print "received data:", data
except Exception:
    s.close()
