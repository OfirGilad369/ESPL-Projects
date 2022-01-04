#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec  3 13:05:25 2021

@author: alex
"""
import socket
import threading
import queue
import sys
import sqlite3
from sqlite3 import Error

import os
import subprocess

bufferSize = 1024



#Server Code
def RecvData(sock,recvPackets):
    while True:
        data,addr = sock.recvfrom(bufferSize)
        recvPackets.put((data,addr))

def RunServer(host):
    mountedUsers = {}
    #host = socket.gethostbyname(socket.gethostname())
    port = 5000
    print('Server hosting on IP-> '+str(host))
    UDPServerSocket = socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM)
    UDPServerSocket.bind((host,port))
    recvPackets = queue.Queue()

    print('Server Running...')

    threading.Thread(target=RecvData,args=(UDPServerSocket,recvPackets)).start()


    #Start
    os.chdir('../')
    original_path = os.getcwd()

    while True:
        data,addr = recvPackets.get()
        data = data.decode('utf-8')
        request = data

        if request.strip() == "":
            continue

        command = request.split()[0]

        if command == 'mount':#mount user
            mountedUsers[addr] = {"mount_satus": request.split()[1], "path": ""}
            print(str(addr) + " mounted the server")

        elif command == 'getPath':#get current path
            message = "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
            UDPServerSocket.sendto(message.encode('utf-8'),addr)

        elif command == 'resetPath':#reset path from client returning to local
            mountedUsers[addr]["path"] = ""

        elif command == 'cd':#change path
            if len(request.split()) == 1:
                mountedUsers[addr]["path"] = "/Server"
                message = "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                UDPServerSocket.sendto(message.encode('utf-8'),addr)
                continue
            else:
                path = ' '.join(request.split()[1:])

                if path == "/":
                    mountedUsers[addr]["path"] = "/Server"
                    message = "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                    UDPServerSocket.sendto(message.encode('utf-8'),addr)
                    continue
                elif path == '.' or path == './':
                    #Do nothing
                    message = "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                    UDPServerSocket.sendto(message.encode('utf-8'),addr)
                    continue
                else:
                    current_path = mountedUsers[addr]["path"]
                    is_avilable_path = False
                    change_path_to_original(original_path)
                    for (root,dirs,files) in os.walk('Server', topdown=True):
                        if root == current_path[1:] + "/" + path:
                            is_avilable_path = True
                            break
                    
                    if is_avilable_path:
                        mountedUsers[addr]["path"] = current_path + "/" + path
                        change_path_to_original(original_path)
                        message = "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                        UDPServerSocket.sendto(message.encode('utf-8'),addr)
                    else:
                        try:
                            os.chdir(path)
                            len_path = len(os.getcwd()) - len(original_path)

                            if len_path > 0:
                                mountedUsers[addr]["path"] = os.getcwd()[-len_path:]
                            else:
                                mountedUsers[addr]["path"] = '/Server'

                            change_path_to_original(original_path)
                            message = "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                            UDPServerSocket.sendto(message.encode('utf-8'),addr)
                        except:
                            message = "No such file or directory\n"
                            message += "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                            UDPServerSocket.sendto(message.encode('utf-8'),addr)

        elif command == 'cp':#copy file
            if len(request.split()) != 3:
                message = "Missing file or destination operands\n"
                message += "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                UDPServerSocket.sendto(message.encode('utf-8'),addr)
                continue

            file_to_copy = request.split()[1]
            dest_locatin = request.split()[2]

            is_avilable_file = False
            for (root,dirs,files) in os.walk('Server', topdown=True):
                if root == mountedUsers[addr]["path"][1:]:
                    if file_to_copy in files:
                        is_avilable_file = True
                    break

            if is_avilable_file:
                os.chdir(mountedUsers[addr]["path"][1:])
                
                seperator = "<SEPERATOR>"
                file_size = os.path.getsize(file_to_copy)

                with open(file_to_copy, "rb") as file:
                    file_data = file.read(file_size)

                change_path_to_original(original_path)
                return_path = "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                message = f"{dest_locatin}{seperator}{file_to_copy}{seperator}{file_size}{seperator}{return_path}"
                
                UDPServerSocket.sendto(message.encode('utf-8'),addr)
                UDPServerSocket.sendto(file_data,addr)
            else:
                message = "File operand doesn't exists\n"
                message += "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                UDPServerSocket.sendto(message.encode('utf-8'),addr)

        else:#execute default command
            os.chdir(mountedUsers[addr]["path"][1:])
            message = subprocess.getoutput(data)
            if message != "":
                message = message + "\n"
            change_path_to_original(original_path)
            message += "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
            UDPServerSocket.sendto(message.encode('utf-8'),addr)
        
    UDPServerSocket.close()
    data_base.close()
#Serevr Code Ends Here

def change_path_to_original(original_path):
    while os.getcwd() != "/":
        os.chdir('../') 
    os.chdir(original_path[1:])


if __name__ == '__main__':
    RunServer(sys.argv[1])
 
