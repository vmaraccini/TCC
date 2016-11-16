from SocketServer import BaseRequestHandler, UDPServer
import thread
import time
import distance

class DistanceHandler(BaseRequestHandler):
    def handle(self):
        print('Got connection from', self.client_address)
        # Get message and client socket
        msg, sock = self.request
        global distance
        resp = str(distance)
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