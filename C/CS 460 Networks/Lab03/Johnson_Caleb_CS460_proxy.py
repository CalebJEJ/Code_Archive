#!/usr/bin/env python3

import argparse

import sys
import itertools
import socket
import os
import threading
from socket import socket as Socket


# A simple web server

# Issues:
# Ignores CRLF requirement
# Header must be < 1024 bytes
# ...
# probabaly loads more

def http_handle(socket_con, address):
    # Save the request received from the connection and decode as ascii
    data_str = socket_con.recv(1024)
    # enforce adding extra line breaks at the end the request in case the user didn't add them
    # without this a "functional" request would receive no answer from the server
    if(data_str[-5:] != b'\r\n\r\n\r\n'):
        data_str += b'\r\n\r\n'
    # turn the data into a usable string
    request = data_str.decode('ascii').rstrip()

    print("\n\nReceived request")
    print("======================")
    print(request.rstrip())
    print("======================")
    reply = b''
    # First Step: Handle Method
    req_lines = request.split('\n')
    # first line is our initial request line
    header_line = req_lines[0].split(' ')
    path = header_line[1]
    if path[0:7] == "http://":
        path = path[7:]
        if (path.find('/') != -1):
            url = path[:path.find('/')]
        else:
            url = path
    else:
        if(path.find('/') != -1):
            url = path[:path.find('/')]
        else:
            url = path

    port = 80
    # default port 80

    print("Creating a new socket...")
    req_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    req_socket.settimeout(20)
    try:
        print("Trying To Connect To: " + url)
        try:
            port = socket.getservbyname(url, 'tcp')
        except:
            port = 80
        # connect our socket to the url given
        req_socket.connect((url, port))

        print("Connected.")

        print("Sending Request")
        req_socket.sendall(data_str)
        print("Sent: ")
        print(data_str)

        print("Receiving Reply...")
        while True:
            buffer = req_socket.recv(1024)
            if not buffer: break
            reply += buffer

    except(socket.gaierror):
        reply = "404 Not Found."
        reply = reply.encode('ascii')

    except(socket.timeout):
        print("Connection Timeout")
        # if the connection times out and the reply is empty inform the client
        if reply.decode('ascii') == "":
            temp = "Connection Timeout Nothing Received."
            reply = temp.encode('ascii')

    # Use the connection socket to send the reply encoded as bytestream
    print("\n\nReplied with")
    print("======================")
    print(reply)
    socket_con.sendall(reply)
    print("======================")
    req_socket.close()
    socket_con.close()

def main():
    # Command line arguments. Use a port > 1024 by default so that we can run
    # without sudo, for use as a real server you need to use port 80.
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', '-p', default=2080, type=int, help='Port to use')
    args = parser.parse_args()

    # Create the server socket (to handle tcp requests using ipv4), make sure
    # it is always closed by using with statement.
    with Socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        # The socket stays connected even after this script ends. So in order
        # to allow the immediate reuse of the socket (so that we can kill and
        # re-run the server while debugging) we set the following option. This
        # is potentially dangerous in real code: in rare cases you may get junk
        # data arriving at the socket.

        # Set socket options
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        # Bind socket to port
        print("Using the port: ", args.port)
        server_socket.bind(('', args.port))

        # Have socket listen
        server_socket.listen(0)

        print("server ready")

        while True:
            # Use the server socket as the connection socket and accept incoming requests
            # This is like file IO and you need to open the server socket as the connection socket

            #with server_socket.accept()[0] as connection_socket:

            connection_socket, client_address = server_socket.accept()
            d = threading.Thread(name=str(client_address), target=http_handle, args=(connection_socket, client_address))
            d.setDaemon(True)
            d.start()

        return 0


if __name__ == "__main__":
    sys.exit(main())