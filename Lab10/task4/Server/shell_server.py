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

        decoded_data = data.decode('utf-8')

        if decoded_data.split()[0] == "sending_file":
            recvPackets.put((data,addr))
            size_of_next_message = int(decoded_data.split()[1])
            data,addr = sock.recvfrom(size_of_next_message)
            recvPackets.put((data,addr))
        else:
            recvPackets.put((data,addr))

def RunServer(host):
    mountedUsers = {}
    usersSharedPaths = {}
    findRequestClient = ""
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
    shared_path = "/Server"

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
            #print shared commands
            if mountedUsers[addr]["path"] != "" and mountedUsers[addr]["mount_satus"] == "shared":
                print(str(addr) + " shared the command: " + data)

                if len(request.split()) == 1:
                    shared_path = "/Server"
                    message = "\033[92m" + shared_path + " $ \033[0m"
                    UDPServerSocket.sendto(message.encode('utf-8'),addr)
                    
                else:
                    path = ' '.join(request.split()[1:])

                    if path == "/":
                        shared_path = "/Server"
                        message = "\033[92m" + shared_path + " $ \033[0m"
                        UDPServerSocket.sendto(message.encode('utf-8'),addr)
                        
                    elif path == '.' or path == './':
                        #Do nothing
                        message = "\033[92m" + shared_path + " $ \033[0m"
                        UDPServerSocket.sendto(message.encode('utf-8'),addr)
                        
                    else:
                        current_path = shared_path
                        is_avilable_path = False
                        change_path_to_original(original_path)
                        for (root,dirs,files) in os.walk('Server', topdown=True):
                            if root == current_path[1:] + "/" + path:
                                is_avilable_path = True
                                break
                        
                        if is_avilable_path:
                            shared_path = current_path + "/" + path
                            change_path_to_original(original_path)
                            message = "\033[92m" + shared_path + " $ \033[0m"
                            UDPServerSocket.sendto(message.encode('utf-8'),addr)
                        else:
                            try:
                                os.chdir(path)
                                len_path = len(os.getcwd()) - len(original_path)

                                if len_path > 0:
                                    shared_path = os.getcwd()[-len_path:]
                                else:
                                    shared_path = '/Server'

                                change_path_to_original(original_path)
                                message = "\033[92m" + shared_path + " $ \033[0m"
                                UDPServerSocket.sendto(message.encode('utf-8'),addr)
                            except:
                                message = "No such file or directory\n"
                                message += "\033[92m" + shared_path + " $ \033[0m"
                                UDPServerSocket.sendto(message.encode('utf-8'),addr)

                

                for user in mountedUsers:
                    if user != addr and mountedUsers[user]["mount_satus"] == "shared" and mountedUsers[user]["path"] != "":
                        user_command = str(addr) + ":" + data + "\n"
                        message = user_command + message
                        UDPServerSocket.sendto(message.encode('utf-8'),user)

            #conncet user to the shared path
            elif mountedUsers[addr]["mount_satus"] == "shared":
                mountedUsers[addr]["path"] = "/Server"
                message = "\033[92m" + shared_path + " $ \033[0m"
                UDPServerSocket.sendto(message.encode('utf-8'),addr)

            #private connection
            else:
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

                                if mountedUsers[addr]["path"] == "":
                                    mountedUsers[addr]["path"] = "/Server"

                                message += "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                                UDPServerSocket.sendto(message.encode('utf-8'),addr)

        elif command == 'cp':#copy file
            if mountedUsers[addr]["path"] != "" and mountedUsers[addr]["mount_satus"] == "shared":
                print(str(addr) + " shared the command: " + data)
                if len(request.split()) != 3:
                    message = "Missing file or destination operands\n"
                    message += "\033[92m" + shared_path + " $ \033[0m"
                    UDPServerSocket.sendto(message.encode('utf-8'),addr)

                    for user in mountedUsers:
                        if user != addr and mountedUsers[user]["mount_satus"] == "shared" and mountedUsers[user]["path"] != "":
                            user_command = str(addr) + ":" + data + "\n"
                            message = user_command + "\033[92m" + shared_path + " $ \033[0m"
                            UDPServerSocket.sendto(message.encode('utf-8'),user)
                    continue

                file_to_copy = request.split()[1]
                dest_locatin = request.split()[2]

                is_avilable_file = False
                for (root,dirs,files) in os.walk('Server', topdown=True):
                    if root == shared_path[1:]:
                        if file_to_copy in files:
                            is_avilable_file = True
                        break

                if is_avilable_file:
                    os.chdir(shared_path[1:])

                    seperator = "<SEPERATOR>"
                    file_size = os.path.getsize(file_to_copy)

                    with open(file_to_copy, "rb") as file:
                        file_data = file.read(file_size)

                    change_path_to_original(original_path)
                    return_path = "\033[92m" + shared_path + " $ \033[0m"
                    message = f"{dest_locatin}{seperator}{file_to_copy}{seperator}{file_size}{seperator}{return_path}"
                    
                    UDPServerSocket.sendto(message.encode('utf-8'),addr)
                    UDPServerSocket.sendto(file_data,addr)
                else:
                    message = "File operand doesn't exists\n"
                    message += "\033[92m" + shared_path + " $ \033[0m"
                    UDPServerSocket.sendto(message.encode('utf-8'),addr)
                
                for user in mountedUsers:
                    if user != addr and mountedUsers[user]["mount_satus"] == "shared" and mountedUsers[user]["path"] != "":
                        user_command = str(addr) + ":" + data + "\n"
                        message = user_command + "\033[92m" + shared_path + " $ \033[0m"
                        UDPServerSocket.sendto(message.encode('utf-8'),user)

            else:
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

        elif command == 'share':#making client directory shared
            if len(request.split()) < 2:
                message = "Missing directory operands\n"

                if mountedUsers[addr]["mount_satus"] == "private":
                    message += "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                else:
                    message += "\033[92m" + shared_path + " $ \033[0m"

                UDPServerSocket.sendto(message.encode('utf-8'),addr)
                continue

            usersSharedPaths[addr] = {"path": ' '.join(request.split()[1:])}
            print(str(addr) + " shared the directory: " + str(usersSharedPaths[addr]["path"]))

            if mountedUsers[addr]["mount_satus"] == "private":
                message = "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
            else:
                message = "\033[92m" + shared_path + " $ \033[0m"

            UDPServerSocket.sendto(message.encode('utf-8'),addr)

        elif command == 'find':#copying a file from one client to another (if file found)
            if len(request.split()) < 2:
                data,addr = recvPackets.get()
                message = "Missing file operands\n"

                if mountedUsers[addr]["mount_satus"] == "private":
                    message += "\033[92m" + mountedUsers[addr]["path"] + " $ \033[0m"
                else:
                    message += "\033[92m" + shared_path + " $ \033[0m"

                UDPServerSocket.sendto(message.encode('utf-8'),addr)
                continue

            findRequestClient = addr
            file_found = False
            file_to_copy = ' '.join(request.split()[1:])

            data,addr = recvPackets.get()
            dest_locatin = data.decode('utf-8')

            for user in usersSharedPaths:
                if user != findRequestClient:
                    message = command + " " + file_to_copy
                    UDPServerSocket.sendto(message.encode('utf-8'),user)
                    message = usersSharedPaths[user]["path"]
                    UDPServerSocket.sendto(message.encode('utf-8'),user)

                    data,addr = recvPackets.get()
                    data = data.decode('utf-8')

                    if data == "true":
                        file_found = True
                        print(str(addr) + " found the file")

                        seperator = "<SEPERATOR>"

                        data,addr = recvPackets.get()
                        data = data.decode('utf-8')
                        file_size = int(data.split()[1])

                        data,addr = recvPackets.get()
                        file_data = data

                        if mountedUsers[findRequestClient]["mount_satus"] == "private":
                            return_path = "\033[92m" + mountedUsers[findRequestClient]["path"] + " $ \033[0m"
                        else:
                            return_path = "\033[92m" + shared_path + " $ \033[0m"

                        message = f"{dest_locatin}{seperator}{file_to_copy}{seperator}{file_size}{seperator}{return_path}"
                        
                        UDPServerSocket.sendto(message.encode('utf-8'),findRequestClient)
                        UDPServerSocket.sendto(file_data,findRequestClient)

                        break
                    else:
                        print(str(addr) + " didn't found the file")

            if file_found == False:
                message = "\033[92m" + mountedUsers[findRequestClient]["path"] + " $ \033[0m"
                UDPServerSocket.sendto(message.encode('utf-8'),findRequestClient)

        else:
            if mountedUsers[addr]["mount_satus"] == "shared" and mountedUsers[user]["path"] != "":
                os.chdir(shared_path[1:])
                message = subprocess.getoutput(data)
                if message != "":
                    message = message + "\n"
                change_path_to_original(original_path)
                message += "\033[92m" + shared_path + " $ \033[0m"
                UDPServerSocket.sendto(message.encode('utf-8'),addr)

                for user in mountedUsers:
                    if user != addr and mountedUsers[user]["mount_satus"] == "shared":
                        user_command = str(addr) + ":" + data + "\n"
                        message = user_command + message
                        UDPServerSocket.sendto(message.encode('utf-8'),user)

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
