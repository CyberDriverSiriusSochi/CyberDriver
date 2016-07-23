import cv2
import socket
import struct
import time

TCP_IP = '10.23.46.38'
TCP_PORT = 4993
BUFFER_SIZE = 1280*720*1

while 1:
    try:    
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((TCP_IP, TCP_PORT))
        data = []
        print('Connection established')
        capture = cv2.VideoCapture(0)
        if not capture.isOpened():
            print('Capture is not opened')
        ret, frame = capture.read()
        while ret:
            print('Frame successfully read')
            bwframe = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            rv, bwbuffer = cv2.imencode('.jpg', bwframe, [cv2.IMWRITE_JPEG_QUALITY ,10])
            print('Image encoded')
            bwstr = bwbuffer.tostring()
            print('String produced')
            if rv:
                sock.send(struct.pack('I', len(bwstr)))
                sock.send(bwbuffer.tostring())
            else:
                print('Frame conversion error')
            ret, frame = capture.read()
            print('Operation completed')
            flag = sock.recv(1).decode('utf-8')
            print('Answer received')
    except socket.error:
        print('Network error (Is server image_proc running?)')
    except Exception:
        print('Exception')
    
    sock.close()
    time.sleep(0.5)                 
