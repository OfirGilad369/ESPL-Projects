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

            seperator = "<SEPERATOR>"
            data_list = data.split(seperator)

            if len(data_list) != 1:#cp case
                file_path = data_list[0]
                file_name = data_list[1]
                file_size = int(data_list[2])
                directory_to_print = data_list[3]

                change_path_to_original(original_path)
                os.chdir('../')
                is_avilable_path = False

                for (root,dirs,files) in os.walk('local', topdown=True):
                    if root == file_path[1:]:
                        is_avilable_path = True
                        break
            
                if is_avilable_path:
                    os.chdir(file_path[1:])

                    with open(file_name, "wb") as file:
                        file_data,addr = sock.recvfrom(file_size)
                        file.write(file_data)

                    unlock_permission = "chmod u+x " + file_name
                    subprocess.getoutput(unlock_permission)
                    change_path_to_original(original_path)
                else:
                    os.chdir('local')
                    print("Could not find the directory to save the file!")
                print(directory_to_print, end="")

            elif data.split()[0] == "find":#find case
                file_to_search = ' '.join(data.split()[1:])
                data,addr = sock.recvfrom(bufferSize)
                shared_path = data.decode('utf-8')

                change_path_to_original(original_path)
                os.chdir('../')
                is_avilable_file = False

                for (root,dirs,files) in os.walk(shared_path[1:], topdown=True):
                    if file_to_search in files:
                        is_avilable_file = True
                        path_of_file = root
                        break
                
                server = (str(sys.argv[1]),5000)
            
                if is_avilable_file:
                    message = "true"
                    sock.sendto(message.encode('utf-8'), server)

                    os.chdir(path_of_file)
                    file_size = os.path.getsize(file_to_search)

                    with open(file_to_search, "rb") as file:
                        file_data = file.read(file_size)
                    
                    message = "sending_file " + str(file_size)
                    sock.sendto(message.encode('utf-8'), server)
                    sock.sendto(file_data, server)

                    change_path_to_original(original_path)
                else:
                    os.chdir('local')
                    message = "false"
                    sock.sendto(message.encode('utf-8'), server)

            else:
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
                        path = ' '.join(request.split()[1:])
                
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
            
            elif command == "cp":
                if len(request.split()) == 3:
                    file_name = request.split()[1]
                    path_to_copy = request.split()[2]
                    if path_to_copy == 'cwd':
                        message = command + " " + file_name + " " + current_path
                        UDPClientSocket.sendto(message.encode('utf-8'), server)
                    else:
                        UDPClientSocket.sendto(request.encode('utf-8'), server)
                else:
                    UDPClientSocket.sendto(request.encode('utf-8'), server)

            elif command == "share":
                if len(request.split()) == 2:
                    path_to_share = request.split()[1]
                    if path_to_share == 'cwd':
                        message = command + " " + current_path
                        UDPClientSocket.sendto(message.encode('utf-8'), server)
                    else:
                        UDPClientSocket.sendto(request.encode('utf-8'), server)
                else:
                    UDPClientSocket.sendto(request.encode('utf-8'), server)

            elif command == "find":
                UDPClientSocket.sendto(request.encode('utf-8'), server)
                message = current_path
                UDPClientSocket.sendto(message.encode('utf-8'), server)

            else:
                UDPClientSocket.sendto(request.encode('utf-8'), server)

    UDPClientSocket.close()
    os._exit(1)
#Client Code Ends Here

def change_path_to_original(original_path):
    while os.getcwd() != original_path:
        os.chdir('../') 


if __name__ == '__main__':
    RunClient(sys.argv[1])
    