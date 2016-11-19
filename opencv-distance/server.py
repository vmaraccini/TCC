from SocketServer import BaseRequestHandler, UDPServer
import thread
import time
import distance
import struct

distance = 0

class DistanceHandler(BaseRequestHandler):
    def handle(self):
        print('Got connection from', self.client_address)
        # Get message and client socket
        msg, sock = self.request
        global distance
        # http://stackoverflow.com/questions/31904047/convert-int-to-2-bytes-of-big-endian
        resp = struct.pack('>H', distance)
        sock.sendto(resp, self.client_address)

serv = UDPServer(('', 20002), DistanceHandler)

def serve():
    serv.serve_forever()

# Create two threads as follows
try:
   thread.start_new_thread(distance.calculate, ())
   thread.start_new_thread(serve, ())
except:
   print "Error: unable to start thread"
while 1:
   pass    