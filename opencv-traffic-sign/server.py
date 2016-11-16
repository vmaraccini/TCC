from SocketServer import BaseRequestHandler, UDPServer
import thread
import time

a = 2

class TimeHandler(BaseRequestHandler):
    def handle(self):
        print('Got connection from', self.client_address)
        # Get message and client socket
        msg, sock = self.request
        global a
        resp = str(a)
        sock.sendto(resp, self.client_address)

serv = UDPServer(('', 20000), TimeHandler)

def calculate():	
    while True:
        global a
        a = a + 1

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