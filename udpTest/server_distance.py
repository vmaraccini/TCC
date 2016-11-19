from SocketServer import BaseRequestHandler, UDPServer
import thread
import time
import struct

class TimeHandler(BaseRequestHandler):
    def handle(self):
        print('Got connection from', self.client_address)
        # Get message and client socket
        msg, sock = self.request
        distance = 20000
        velocity = 40/3.6*100
        resp = chr(int(distance % 256)) + chr(int(distance / 256))
        resp = resp + chr(int(velocity % 256)) + chr(int(velocity / 256))
        sock.sendto(resp, self.client_address)

serv = UDPServer(("localhost", 20002), TimeHandler)

def calculate():
    while True:
        pass

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
