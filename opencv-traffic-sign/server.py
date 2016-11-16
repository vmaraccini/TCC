import realtime
from SocketServer import BaseRequestHandler, UDPServer
import thread
import json
import struct

digits = (0)

class TrafficSignHandler(BaseRequestHandler):
    def handle(self):
        print('Got connection from', self.client_address)
        # Get message and client socket
        msg, sock = self.request
        global digits
        speed = int(str(''.join(digits))) 
        # http://stackoverflow.com/questions/31904047/convert-int-to-2-bytes-of-big-endian
        resp = struct.pack('>H', speed)
        sock.sendto(resp, self.client_address)

serv = UDPServer(('', 20001), TrafficSignHandler)

def serve():
    serv.serve_forever()

# Create two threads as follows
try:
   thread.start_new_thread(realtime.calculate, ())
   thread.start_new_thread(serve, ())
except:
   print "Error: unable to start thread"
while 1:
   pass    