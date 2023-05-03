#include <iostream>

#include <vector>

#include <string>

#include <cstring>

#include <unistd.h>

#include <chrono>

#include <ctime>

#include <netdb.h>

#include <map>

#include <thread>

#include <fstream>

#include <vector>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <pthread.h>

#include <vector>

 



int get_client_count=0;



// Define the maximum number of CONNECTIONS it can have at a time

#define CONNECTIONS_TIME 50



// Define message buffer size

#define BUFFER_CAPACITY 1024



// Define broadcast time limit in seconds

#define BROADCAST_TIME 1800



//Define the number of threads

std::vector<std::thread> threads;

 

//Define the number of clients

std::vector<int> clients_count;





// Define client information struct

struct Clients_Info {

    std::string username;

    std::string password;

    std::string ip_address;

    int tcp_socket;

};



std::vector<Clients_Info> clients;



//Thread to handle clients

pthread_mutex_t Mutex_clients = PTHREAD_MUTEX_INITIALIZER;



// Declare UDP socket descriptor

int udp_socket;

//Declare TCP socket 

int tcp_socket;



// Declare function prototypes

void handle_new_client(int new_Client_socket);



//Get the IP address and port of client connected

std::vector<std::string>Get_IP_Port()

{

char New_Client_Buffer[256];

std::vector<std::string> Port_IP_value;

	struct hostent *Client_push;

	int hostname;

	struct in_addr **addr_list;

	sockaddr_in addr {};

	socklen_t len = sizeof(addr);

	hostname = gethostname(New_Client_Buffer, sizeof(New_Client_Buffer));

	Client_push = gethostbyname(New_Client_Buffer);

	if (Client_push == NULL) {

		perror("gethostbyname error");

		exit(1);

	}

	addr_list = (struct in_addr **)Client_push->h_addr_list;

	        for(int i=0;i<get_client_count;i++){

		 Port_IP_value.push_back("Client "+ std::to_string(i+1) + ":" + inet_ntoa(*addr_list[0]));

		}

		std::cout << "Port number: " << ntohs(addr.sin_port) << std::endl;

return Port_IP_value;

}



//Function to accept new clients

void accept_new_clients(int socket) {

    while (true) {

        int new_Client_socket, size_client_addr;

        struct sockaddr_in new_client_addr;



        // Accept new client connection

        size_client_addr = sizeof(new_client_addr);

        if ((new_Client_socket = accept(socket, (struct sockaddr*)&new_client_addr, (socklen_t*)&size_client_addr)) < 0) {

            std::cerr << "Unable to accept new connection." << std::endl;

            continue;

        }

        // Create a new thread to handle the new client connection

        std::thread client_thread(handle_new_client, new_Client_socket);

        // detach thread to run independently

         client_thread.detach();

        clients_count.push_back(new_Client_socket);

    }

}





using namespace std;

int main() {

    int tcp_socket;

    struct sockaddr_in tcp_addr;



    // Create TCP socket

    if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        std::cerr << "Creating TCP socket failed." << std::endl;

        exit(EXIT_FAILURE);

    }



    // Bind socket to address

    tcp_addr.sin_family = AF_INET;

    tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    tcp_addr.sin_port = htons(4400); 

    if (bind(tcp_socket, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr)) < 0) {

        std::cerr << "Binding TCP socket failed." << std::endl;

        exit(EXIT_FAILURE);

    }



    // Listen for incoming CONNECTIONS_TIME

    if (listen(tcp_socket, CONNECTIONS_TIME) < 0) {

        std::cerr << "Listening for CONNECTIONS_TIME failed." << std::endl;

        exit(EXIT_FAILURE);

    }



    // Accept incoming CONNECTIONS_TIME in separate threads

    for (int i = 0; i < CONNECTIONS_TIME; i++) {

        threads.push_back(std::thread(accept_new_clients, tcp_socket));

    }



    // Join threads

    for (auto& t1 : threads) {

        t1.join();

    }



    return 0;

}



//Handling new clients

void handle_new_client(int new_Client_socket) {

    char buffer[1024];

    int received_bytes = recv(new_Client_socket, buffer, 1024, 0);

    std::string request(buffer, received_bytes);



    // Respond with login/signup prompt

    std::string client_response = "Welcome to the server! Please choose an option: 1) login || 2) signup\n";

    send(new_Client_socket, client_response.c_str(), client_response.length(), 0);



    while (true) {

        received_bytes = recv(new_Client_socket, buffer, 1024, 0);

        std::string choice(buffer, received_bytes);



        if (choice == "Login") {

            // Ask for username and password

            client_response = "Please enter your username:\n";

            send(new_Client_socket, client_response.c_str(), client_response.length(), 0);



            received_bytes = recv(new_Client_socket, buffer, 1024, 0);

            if (received_bytes < 0) {

                // error handling

                return;

            }

            std::string username(buffer, received_bytes);



            client_response = "Please enter your password:\n";

            send(new_Client_socket, client_response.c_str(), client_response.length(), 0);



            received_bytes = recv(new_Client_socket, buffer, 1024, 0);

            if (received_bytes < 0) {

                // error handling

                return;

            }

            std::string password(buffer, received_bytes);



             // Check auth_flag from CSV file

            std::ifstream file("clients_info.csv");

            std::string tuples;

            bool auth_flag = false;

            while (std::getline(file, tuples)) {

                size_t pos = tuples.find(',');

                if (pos != std::string::npos) {

                    std::string file_username = tuples.substr(0, pos);

                    std::string file_password = tuples.substr(pos+1);

                    if (file_username == username && file_password == password) {

                        auth_flag = true;

                        break;

                    }

                }

            }

            file.close();



            if (auth_flag) {

                get_client_count++;

                client_response = "Login is successful!!!\n";

                

                send(new_Client_socket, client_response.c_str(), client_response.length(), 0);

                

                 // Show options to the authenticated client

        client_response = "You are successfully authenticated. Choose an option:\n1. List of active clients\n2. Broadcast a message\n";

        send(new_Client_socket, client_response.c_str(), client_response.length(), 0);



        received_bytes = recv(new_Client_socket, buffer, 1024, 0);

        if (received_bytes < 0) {

            // error handling

            return;

        }

        std::string option(buffer, received_bytes);



        if (option == "1") {

           vector<string> a=Get_IP_Port();

      

            // List active clients that are connected to server

            std::string active_clients_list = "Active Clients are :\n";

            for (const auto& client : a) {

                active_clients_list +=   client +"\n";

            }

            send(new_Client_socket, active_clients_list.c_str(), active_clients_list.length(), 0);

            received_bytes = recv(new_Client_socket, buffer, 1024, 0);

        if (received_bytes < 0) {

            // error handling

            return;

        }

        std::string ipadd(buffer, received_bytes);

        



        } else if (option == "2") {

            // Broadcast a message

            client_response = "Enter the message to broadcast:\n";

            send(new_Client_socket, client_response.c_str(), client_response.length(), 0);



            received_bytes = recv(new_Client_socket, buffer, 1024, 0);

            if (received_bytes < 0) {

                // error handling

                return;

            }

            std::string msg(buffer, received_bytes);

            std::string broadcast_message = "Message Broadcasted by \n" + std::to_string(new_Client_socket-2) + "\nMessage ::\n"+msg; 

             for (int new_Client_socket : clients_count) {

                    std::cout << " client conected is : " <<  new_Client_socket << "\n";

            send(new_Client_socket, broadcast_message.c_str(), broadcast_message.size(), 0);

                }



            client_response = "Message has been sent successfully!\n";

            send(new_Client_socket,client_response.c_str(), client_response.length(), 0);

} 

            } else {

                client_response = "Login failed due to Incorrect username or password.\n";

                send(new_Client_socket, client_response.c_str(), client_response.length(), 0);

            }

        } 

            else if (choice == "Signup") {

            // Ask for new username and password

            client_response = "Please enter a new username:\n";

            send(new_Client_socket, client_response.c_str(), client_response.length(), 0);



            received_bytes = recv(new_Client_socket, buffer, 1024, 0);

            if (received_bytes < 0) {

                // error handling

                return;

            }

            std::string new_username(buffer, received_bytes);



            // Check if username already exists in CSV file

            std::ifstream file("clients_info.csv");

            std::string line;

            bool username_present = false;

            while (std::getline(file, line)) {

                size_t pos = line.find(',');

                if (pos != std::string::npos) {

                std::string username_from_csv_file = line.substr(0, pos);

                if (new_username == username_from_csv_file) {

                username_present = true;

                    break;

                    }

                    }

                        }

                file.close();



                 if (username_present) {

            client_response = "Username already exists. Please choose another username.\n";

            send(new_Client_socket, client_response.c_str(), client_response.length(), 0);

        } else {

            client_response = "Please enter a new password:\n";

            send(new_Client_socket, client_response.c_str(), client_response.length(), 0);



            received_bytes = recv(new_Client_socket, buffer, 1024, 0);

            if (received_bytes < 0) {

                // error handling

                return;

            }

            std::string new_password(buffer, received_bytes);



            client_response = "Please confirm your password:\n";

            send(new_Client_socket, client_response.c_str(), client_response.length(), 0);



            received_bytes = recv(new_Client_socket, buffer, 1024, 0);

            if (received_bytes < 0) {

                // error handling

                return;

            }

            std::string confirmed_password(buffer, received_bytes);



            if (new_password != confirmed_password) {

                client_response = "Entered passwords are not matching. Please try again later...\n";

                send(new_Client_socket, client_response.c_str(), client_response.length(), 0);

            } else {

                // Write username and password to CSV file

                std::ofstream outfile;

                outfile.open("clients_info.csv", std::ios_base::app);

                outfile << new_username << "," << confirmed_password << std::endl;

                outfile.close();



                client_response = "Signup is successful. Please login to communicate...\n";

                send(new_Client_socket, client_response.c_str(), client_response.length(), 0);

            }

        }



        } else {

            // Invalid choice

            client_response = "Invalid option. Please choose login or signup\n";

            send(new_Client_socket, client_response.c_str(), client_response.length(), 0);

        }

    }

}



