//Name: Nandini Talwar
//Purpose: Connects the lock to the web server to enable GPS tracking, collect data from GPS, and collect data from accelerometer.
//Edited Open Source Code on GitHub

#!/usr/bin/env python3
import socket
import sys
import json
import requests

def parse(string):
    str_list = string.split("\n")
    data = {"gps" : [0,0], "axis" : [0,0,0], "uid" : 0}
    bike_id = ""
    for i in str_list:
        if i == "*" or i == "":
            continue
        k,v = i.split("=")
        if k == "id":
            bike_id = v
        elif k == "uid":
            data["uid"] = int(v, 16)
        elif k == "lng":
            data["gps"][0] = float(v)
        elif k == "lat":
            data["gps"][1] = float(v)
        elif k == "x":
            data["axis"][0] = float(v)
        elif k == "y":
            data["axis"][1] = float(v)
        elif k == "z":
            data["axis"][2] = float(v)
    return bike_id, data

def main():
    while True:
        try:
            serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            host, port = socket.gethostname(), 5566
            serversocket.bind((host, port))
            serversocket.listen(256)
            clientsocket, addr = serversocket.accept()
            print("Got a connection from %s" % str(addr))
            while True:
                recv_msg = ""
                while "*" not in recv_msg:
                    recv_msg += clientsocket.recv(1024).decode()
                bike_id, data = parse(recv_msg)

                url = 'http://bike.csie.org:8080/map/' + bike_id + "/"
                response = requests.post(url, json=data)
                response = response.text.split(",")
                if "available" in response[0]:
                    available = True if ("true" in response[0]) else False
                    is_stolen = True if ("true" in response[1]) else False
                else:
                    is_stolen = True if ("true" in response[0]) else False
                    available = True if ("true" in response[1]) else False
                if available:
                    clientsocket.send("1".encode())
                elif is_stolen:
                    clientsocket.send("2".encode())
                else:
                    clientsocket.send("0".encode())

        except socket.error:
            clientsocket.close()
            serversocket.close()
            pass

if __name__ == '__main__':
    sys.exit(main())
