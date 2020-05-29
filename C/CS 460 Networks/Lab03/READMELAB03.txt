Works from Linux Command Window

Part 1: Simple Web Server
The simple server can be started with the commands:

python3 Johnson_Caleb_CS460_Lab03_web_server.py
    OR
python3 Johnson_Caleb_CS460_Lab03_web_server.py 
-p <port # of choice>
------------------------------------------------------------------

Part 2: Proxy Server
The proxy server can be started with the commands:

python3 Johnson_Caleb_CS460_proxy.py
    OR
python3 Johnson_Caleb_CS460_proxy.py
-p <port # of choice>
-------------------------------------------------------------------
Connecting to the server:
telnet localhost <port choosen / 2080 by default>

Sending a request:
<METHOD> <DESTINATION> HTTP/1.0 (other http untested)

Examples to try:
GET http://eunops.org/ HTTP/1.0