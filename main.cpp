#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
using namespace std;

/*
REFERENCES:
    https://www.geeksforgeeks.org/socket-programming-cc/
    https://cs.berry.edu/~nhamid/p2p/index.html
    https://beej.us/guide/bgnet/html/single/bgnet.html#syscalls


*/



void startConnection();
void startListening();

int main(){
    string usrOpt;

    while(true){
        cout << "\n\n*\n\n*\n\nIt's a secret!" << endl;
        cout << "Welcome to Dylan's P2P IM!" << endl;
        cout << "Enter '1' to connect, or '0' to wait for a connection. 'q' to quit." << endl;
        cin >> usrOpt;
        if(usrOpt == "q"){
            break;
        }else if(usrOpt == "1"){
            startConnection();
        }else{
            startListening();
        }
    }
    

}

void startConnection(){
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char const *hello = "Hello from client"; 
     
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(8080); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return; 
    } 
    cout << "PEER CONNECTED!" << endl;
    cout << "Enter 'q' to quit" << endl;
    
    bool firstTime = true;

    while(true){
        char msg[1024];
        char buffer[1024] = {0};
        cout << "you: ";
        string grabinput;
        
        if(firstTime){
            cin.ignore();
            firstTime = false;
        }
        
        getline(cin, grabinput);
        strcpy(msg, grabinput.c_str());

        send(sock , msg , strlen(msg) , 0 ); 

        if(msg[0] == 'q'){
            cout << "quitting..." << endl;
            break;
        }

        valread = read( sock , buffer, 1024);

        if(buffer[0] == 'q'){
            cout << "PEER DISCONNECTED: quitting" << endl;
            break;
        }

        cout << "them: " << buffer << endl;

    }
    
    shutdown(sock, SHUT_RDWR);
    close(sock);
    cout << "DISCONNECTED!" << endl;
    return;
}

void startListening() {
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
     
    char const *hello = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( 8080 ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    cout << "Waiting for connection..." << endl;

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    cout << "PEER CONNECTED!" << endl;
    cout << "Enter 'q' to quit" << endl;

    bool firstTime = true;


    while(true){
        char buffer[1024] = {0};
        
        valread = read( new_socket , buffer, 1024); 

        if(buffer[0] == 'q'){
            cout << "PEER DISCONNECTED: quitting" << endl;
            break;
        }
    
        cout << "them: " << buffer << endl;
        
        
        char msg[1024];
        
        cout << "you: ";

        if(firstTime){
            cin.ignore();
            firstTime = false;
        }

        
        string grabinput;
        getline(cin, grabinput);
        strcpy(msg, grabinput.c_str());

        send(new_socket , msg , strlen(msg) , 0 );

        if(msg[0] == 'q'){
            cout << "quitting..." << endl;
            break;
        }

    }
    


    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);
    shutdown(new_socket, SHUT_RDWR);
    close(server_fd);

    cout << "DISCONNECTED!" << endl;
    return;
}



void test(){
    //create sockets; send/listen
    int listenSock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addressListen;
    
    addressListen.sin_family = AF_INET; 
    addressListen.sin_addr.s_addr = INADDR_ANY; 
    addressListen.sin_port = htons( 8080 );

    bind(listenSock, (struct sockaddr *)&addressListen, sizeof(addressListen));

    listen(listenSock, 1);

    int mySock = accept(listenSock, (struct sockaddr *)&addressListen, (socklen_t*)&addressListen);

    char data[5000];

    read(listenSock, data, 5000);

}