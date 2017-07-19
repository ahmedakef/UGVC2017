# this code to chat between to process in the same machine

import socket               # Import socket module

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)         # Create a socket object
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # make the port reusable after closing it quickly

host = socket.gethostname() # Get local machine name
port = 12345                # Reserve a port for your service.
s.bind((host, port))        # Bind to the port

s.listen(1)                 # Now wait for client connection.
c, addr = s.accept()        # Establish connection with client.
print 'Got connection from', addr
c.send('Thank you for connecting')

while True:
    data=c.recv(1024)
    print("i have recieved %s" % data )

    c.send('hey client , i have recieved %s' % data)

c.close()                # Close the connection
s.close()
