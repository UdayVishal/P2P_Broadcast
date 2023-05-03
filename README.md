# P2P_Broadcast
                                                 Peer-to-peer messaging and broadcast message
This Linux distribution client-server program was created in C/C++. The goal of this project is to create a client-server application that allows for client-to-client communication without the use of a server. Clients can connect to the server, log in with a password, and view the list of clients that are currently active. Once the client chooses option like login, he will be displayed with two options like List of active clients and Broadcast message. If the client chooses option like signup, he will be asked to enter his new username and new password and confirm password to store in the csv file which we used as a database. Any current client can be selected as the connection point, and the distant client will be prompted to accept the new connection. Once a connection has been established successfully, clients can communicate with one another without the server. Additionally, a client can ask the server to send a message to every client in its database (which is a csv file) by making a broadcast request, so that the message will be transferred to every client connected to the server.

Requirements
* C++ compiler
* Virtual machine
* CSV file 

Usage
  Server
* To compile the server source code use the command “ g++ server.cpp -o server -pthread ”
* To run the server, navigate to the server directory and run the following command. “ ./server ”.
* The server will listen on port 4400 and allow at least 50 connections simultaneously.
![image](https://user-images.githubusercontent.com/86564176/235823505-202ff7d2-28ff-41a2-a248-fc2e11021d6a.png)
   
  Client
* To compile the client source code use the command “g++ client.cpp -o client -pthread ”
* To run the client, navigate to the client directory and run the following command. “ ./client <IP address> <port> ”.
* The client will connect to the server with a fixed IP address and port 4400.
![image](https://user-images.githubusercontent.com/86564176/235823566-9631c0d6-7190-4723-b6b0-77c42727c076.png)


Working of Server and Client
* Client is asked to Login or Signup. 
* The server creates a TCP and UDP socket. TCP socket is used to communicate over clients, and UDP socket is used to broadcast messages.
* The server and the client run on the same port 4400.
* Once the client selects signup his username and password will be stored in the csv file we created, and this csv file works as a database.
* Once the client is authenticated i.e., if he is successfully logged in, it can choose to see the list of active clients or broadcast a message using a UDP socket.
* To connect to another client and start peer-to-peer messaging, the client can request the server to let connect to one of the clients by typing their IP address.
* To disconnect the P2P connection, the client can use a special command (ctrl+c).
* To receive the broadcast message, the client needs to have a UDP socket (to receive the broadcast message) open with the server, once it is authenticated by the server.
* When there is no P2P connection active, the client can request the server to close the connection using a special command (ctrl+x).
![image](https://user-images.githubusercontent.com/86564176/235823584-746e973c-5cb6-4f20-a8fa-2a28cfd75eed.png)

![image](https://user-images.githubusercontent.com/86564176/235823602-ee7cc4ed-32db-461b-bef2-6d2c76d01060.png)

Libraries  Used
* #include <stdio.h>
* #include <sys/socket.h>
* #include <arpa/inet.h>
* #include <netinet/in.h>
* #include <unistd.h>
* #include <pthread.h>
* #include <iostream>
* #include <cstring>
* #include <string.h>
* #include <string>
* #include <chrono>
* #include <ctime>
* #include <netdb.h>
* #include <map>
* #include <thread>
* #include <fstream>
* #include <arpa/inet.h>
* #include <pthread.h>
* #include <vector>

Building of Project
* Use Makefiles to build your project.
* To build the server, navigate to the server directory and run the following command: “ Make Server ”
* To build the client, navigate to the client directory and run the following command: “ Make Client ”





     
