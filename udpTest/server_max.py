from SocketServer import BaseRequestHandler, UDPServer
import thread
import time
import struct

class TimeHandler(BaseRequestHandler):
    def handle(self):
        print('Got connection from', self.client_address)
        # Get message and client socket
        msg, sock = self.request
        resp = chr(50)
        print(resp)
        sock.sendto(resp, self.client_address)

serv = UDPServer(("localhost", 20001), TimeHandler)

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
