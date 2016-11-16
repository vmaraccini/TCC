import realtime
from SocketServer import BaseRequestHandler, UDPServer
import thread
import json

digits = (0)

class TrafficSignHandler(BaseRequestHandler):
    def handle(self):
        print('Got connection from', self.client_address)
        # Get message and client socket
        msg, sock = self.request
        global digits
        resp = json.dumps(digits)
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