#!/usr/bin/python           # This is client.py file

import socket               # Import socket module

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)         # Create a socket object

port = 12345                # Reserve a port for the service.
host = socket.gethostname() # host name or adress of the \
                            # target machine


s.connect((host, port))
print s.recv(1024)
while True:
    to_sent=raw_input()
    if to_sent=="":
        continue
    s.send(to_sent)
    print("I send    : %s" % to_sent)
    data= s.recv(1024)
    print("I recieve : %s" % data)

s.close()                     # Close the socket when done
