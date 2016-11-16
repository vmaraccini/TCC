import cv2
import recognize
import process
import tools
from copy import deepcopy
from socketserver import BaseRequestHandler, UDPServer
import thread
import time

lastKnownVelocity = None
def captureAndRecognize(videoCapture):
    ret, frame = videoCapture.read()

    image = deepcopy(frame)
    preprocessed = process.processImage(image)

    cv2.waitKey(10)

    digits = recognize.recognize(preprocessed, image, image)

    lastKnownVelocity = tools.getSpeedLimitFromDigits(digits, lastKnownVelocity)

    display = tools.printRecognizedDigits(frame, lastKnownVelocity)
    cv2.imshow("Capture", display)

class TimeHandler(BaseRequestHandler):
    def handle(self):
        print('Got connection from', self.client_address)
        # Get message and client socket
        msg, sock = self.request
        resp = time.ctime()
        sock.sendto(resp.encode('ascii'), self.client_address)

cap = cv2.VideoCapture(0)
serv = UDPServer(('', 20000), TimeHandler)

def calculate():	
    while True:
         captureAndRecognize(cap)

def serve():
    serv.serve_forever()

# Create two threads as follows
try:
   thread.start_new_thread(calculate, ())
   thread.start_new_thread(serve, ())
except:
   print "Error: unable to start thread"
while 1:
   pass    