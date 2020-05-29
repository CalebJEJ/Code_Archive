#!/usr/bin/env python3

import argparse

import sys
import itertools
import socket
from socket import socket as Socket


# A simple web server

# Issues:
# Ignores CRLF requirement
# Header must be < 1024 bytes
# ...
# probabaly loads more

def http_handle(request):
    #default as a bad request
    reply_msg = "Bad Request. \n"
    status_code = "400"
    http_version = "HTTP/1.0"
    reply_body = ""
    headers_or_data = False
    # First Step: Handle Method
    req_lines = request.split('\n')
    # first line is our initial request line
    request_line = req_lines[0].split(' ')

    if len(req_lines) > 1:
        headers_or_data = True

    if len(request_line) != 3:
        reply = http_version + " " + status_code + " " + reply_msg + "\n"
        return reply

    method = request_line[0]
    url = request_line[1]
    http_version = request_line[2]

    if http_version != "HTTP/1.0":
        reply_msg = "Version of HTTP not implemented yet."
        status_code = "501"

    http_check = url[0 : 7]

    # see if the request is an http:// request location
    if http_check == "http://":
        # strip out the http:// and ip address
        url = url[7:]
        url = url[url.find('/'):]

    # use a try except block to open file and handle errors
    try:
        # attempt to read in the file
        if url == "/":
            url = "index.html"
        http_file = open(url, 'r')
        reply_body = http_file.read()
    except FileNotFoundError:
        status_code = "404"
        reply_msg = "File not found"
        reply = http_version + " " + status_code + " " + reply_msg + "\n"
        return reply

    if method == "GET":
        status_code = "200"
        reply_msg = "OK"

    if method == "POST":
        reply_msg = "POST requests are not implemented yet."
        status_code = "501"

    if method == "HEAD":
        reply_msg = "OK."
        status_code = "200"

    if method == "PUT":
        reply_msg = "PUT requests are not implemented yet."
        status_code = "501"

    reply = http_version + " " + status_code + " " + reply_msg + " \n\n " + reply_body + "\n"

    http_file.close()

    return reply



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
            with server_socket.accept()[0] as connection_socket:
                # Save the request received from the connection and decode as ascii
                data_str = connection_socket.recv(1024).decode('ascii')
                request = data_str
                reply = http_handle(request)

                # Generate a reply by sending the request received to http_handle()
                # See function http_handle()

                # Use the connection socket to send the reply encoded as bytestream
                reply = reply.rstrip()
                reply_encoded = reply.encode('ascii')

                print("\n\nReceived request")
                print("======================")
                print(request.rstrip())
                print("======================")
                print("\n\nReplied with")
                print("======================")
                print(reply)
                connection_socket.send(reply_encoded)
                print("======================")


if __name__ == "__main__":
    sys.exit(main())