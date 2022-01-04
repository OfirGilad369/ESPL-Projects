#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Dec  8 13:03:13 2021

@author: alex
"""
import socket
import threading
import sys
import random
import os

import subprocess
from time import sleep

bufferSize = 1024
original_path = os.getcwd()

#Client Code
def ReceiveData(sock):
    while True:
        try:
            data,addr = sock.recvfrom(bufferSize)
            data = data.decode('utf-8')
            print(data, end="")

        except:
            pass

def RunClient(serverIP):
    host = socket.gethostbyname(socket.gethostname())
    port = random.randint(6000,10000)
    print('Client IP->'+str(host)+' Port->'+str(port))
    server = (str(serverIP),5000)
    UDPClientSocket = socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM)
    UDPClientSocket.bind((host,port))
    #name = 'make connection to the server'
    #UDPClientSocket.sendto(name.encode('utf-8'),server)
    threading.Thread(target=ReceiveData,args=(UDPClientSocket,)).start()


    # Start
    serverPath = f"{serverIP}:5000:"
    current_path = "/local"
    isMounted = False
    isExecutingOnServer = False

    while True: 
        if not isExecutingOnServer:
            print("\033[92m" + current_path + " $" + "\033[0m", end=" ")       
            request = input()

            if request.strip() == "":
                continue

            command = request.split()[0]

            if command == "mount":
                if len(request.split()) != 3:
                    print("Mount failed! Reason: invalid mount action.")
                    continue
                
                mountVisibility = request.split()[1]

                if mountVisibility != 'private' and mountVisibility != 'shared':
                    print("Mount failed! Reason: invalid visibility mode.")
                    continue

                if not isMounted:
                    
                    mountPath = request.split()[2]

                    if mountPath == serverPath + "/Server":
                        message = command + " " + mountVisibility
                        UDPClientSocket.sendto(message.encode('utf-8'), server)
                        isMounted = True
                    else:
                        print("Mount failed! Reason: invalid path.")
                        continue
                else:
                    print("Mount failed! Reason: path already mounted.")

            elif command == "unmount":
                if isMounted:
                    mountPath = request.split()[1]

                    if mountPath == serverPath + "/Server":
                        isMounted = False
                        print("Unmount succeeded.")

                    else:
                        print("Unmount failed! Reason: path not found.")
                else:
                    print("Unmount failed! Reason: no path was mounted.")

            elif command == 'cd':
                if len(request.split()) == 1:
                    current_path = '/local'
                    change_path_to_original(original_path)
                    continue
                else:
                    path = ' '.join(request.split()[1:])

                    if path == '/':
                        current_path = '/local'
                        change_path_to_original(original_path)
                        continue
                    elif path == '.' or path == './':
                        #Do nothing
                        continue
                    elif path.split("/")[0] == serverPath:
                        if not isMounted:
                            print("Failed changing directory to Server! Reason: server not mounted.")
                            continue
                        message = command + " " + '/'.join(path.split("/")[1:])
                        UDPClientSocket.sendto(message.encode('utf-8'), server)
                        isExecutingOnServer = True
                        continue
                    else:
                        is_avilable_path = False
                        change_path_to_original(original_path)
                        os.chdir('../')
                        for (root,dirs,files) in os.walk('local', topdown=True):
                            if root == current_path[1:] + "/" + path:
                                is_avilable_path = True
                                break
                        
                        os.chdir('local')
                        if is_avilable_path:
                            current_path = current_path + "/" + path
                            os.chdir(path)
                        else:
                            try:
                                os.chdir(path)

                                len_path = len(os.getcwd()) - len(original_path)

                                if len_path > 0:
                                    current_path = '/local' + os.getcwd()[-len_path:]
                                else:
                                    current_path = '/local'
                                    change_path_to_original(original_path)
                            except:
                                print("No such file or directory")
            else:
                result = subprocess.getoutput(request)
                if result != "":
                    print(result)
                    
        else:
            sleep(0.5)      
            request = input()

            if request.strip() == "":
                message = "getPath"
                UDPClientSocket.sendto(message.encode('utf-8'), server)
                continue 

            command = request.split()[0]

            if command == 'cd' and len(request.split()) != 1:
                path = request.split()[1]
                
                if path.split(":")[0] == "local":
                    localPath = ' '.join(request.split(":")[1:])
                    change_path_to_original(original_path)
                    os.chdir('../')
                    is_avilable_path = False

                    for (root,dirs,files) in os.walk('local', topdown=True):
                        if root == localPath[1:]:
                            is_avilable_path = True
                            break
                
                    if is_avilable_path:
                        os.chdir(localPath[1:])
                        current_path = localPath
                        isExecutingOnServer = False
                        message = "resetPath"
                        UDPClientSocket.sendto(message.encode('utf-8'), server)
                        continue
                    
                    os.chdir(current_path[1:])
                    print("No such file or directory")
                    message = "getPath"
                    UDPClientSocket.sendto(message.encode('utf-8'), server)
                else:
                    UDPClientSocket.sendto(request.encode('utf-8'), server) 
                           
            else:
                UDPClientSocket.sendto(request.encode('utf-8'), server)

    UDPClientSocket.close()
    os._exit(1)
#Client Code Ends Here

def change_path_to_original(original_path):
    while os.getcwd() != "/":
        os.chdir('../') 
    os.chdir(original_path[1:])


if __name__ == '__main__':
    RunClient(sys.argv[1])
    