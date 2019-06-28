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

void init(int initType){

    //Generic socket setup.
    //Can go either way.... listen/connect
    int sock_listen;
    int sock_peer;
    struct sockaddr_in address;
    int addrlen = sizeof(address);    
       
    sock_listen = socket(AF_INET, SOCK_STREAM, 0);       
    
    address.sin_family = AF_INET; 
    address.sin_port = htons(65500); 

    bind(sock_listen, (struct sockaddr *)&address, sizeof(address));

    //0 if listening, 1 if connecting.
    if(initType == 0){
        //any incoming IP address will be accepted.
        address.sin_addr.s_addr = INADDR_ANY;

        sock_listen = socket(AF_INET, SOCK_STREAM, 0);
    }else{
        sock_peer = socket(AF_INET, SOCK_STREAM, 0);

        string target_IP;
        cout << "Enter IP of Peer: ";
        cin >> target_IP;

        // Convert IPv4 and IPv6 addresses from text to binary form 
        if(inet_pton(AF_INET, target_IP.c_str(), &address.sin_addr)<=0)  
        { 
            printf("\nInvalid address/ Address not supported \n"); 
            return; 
        }

        if(connect(sock_peer, (struct sockaddr *)&address, sizeof(address)) == -1 ){
        cout << "Connection Failed!" << endl;
        return;
    }
   }
}

void startConnection(){
    int sock = 0;
    int bytesRead = 0; 
    struct sockaddr_in peer_address; 
     
    sock = socket(AF_INET, SOCK_STREAM, 0);
   
    peer_address.sin_family = AF_INET; 
    peer_address.sin_port = htons(65500); 
       
    string target_IP;
    cout << "Enter IP of Peer: ";
    cin >> target_IP;

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, target_IP.c_str(), &peer_address.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return; 
    } 

    if(connect(sock, (struct sockaddr *)&peer_address, sizeof(peer_address)) == -1 ){
        cout << "Connection Failed!" << endl;
        return;
    }

    cout << "PEER CONNECTED!" << endl;
    cout << "Enter '<?q' to quit\nEnter '<?s [filepath]' to send a file" << endl;
    cin.ignore();

    int recvFlag = MSG_DONTWAIT;

    while(true){
        char msg_out[1024] = {0};
        char msg_in[1024] = {0};

        while(true){
            if(recv(sock, msg_in, sizeof(msg_in), recvFlag) > 0){
                cout << "Them: " << msg_in << endl;
            }else{
                break;
            }
        }
        
        if(msg_in[0] == '<' && msg_in[1] == '?' && msg_in[3] == 'q'){
            cout << "PEER DISCONNECTED: quitting" << endl;
            break;
        }


        if(msg_in[0] == '<' && msg_in[1] == '?' && msg_in[3] == 's'){
            cout << "PEER DISCONNECTED: quitting" << endl;
            break;
        }

        cout << "You: ";
        string grabinput;

        getline(cin, grabinput);
        strcpy(msg_out, grabinput.c_str());

        if(grabinput.size() == 0){
            //recvFlag = 0;
            cout << "Refreshing..." << endl;
        }else{
            recvFlag = MSG_DONTWAIT;
            if(send(sock, msg_out, sizeof(msg_out), 0) <= 0){
                cout << "send failed!!" << endl;
            }
        }

        

        if(msg_out[0] == 'q'){
            cout << "quitting..." << endl;
            break;
        }
    }
    
    shutdown(sock, SHUT_RDWR);
    close(sock);
    cout << "DISCONNECTED!" << endl;
    return;
}

void startListening() {
    int sock_listen;
    int sock_peer;
    struct sockaddr_in address;
    int addrlen = sizeof(address);    
       
    sock_listen = socket(AF_INET, SOCK_STREAM, 0);       
    
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(65500); 
    
    bind(sock_listen, (struct sockaddr *)&address, sizeof(address));

    listen(sock_listen, 3);

    cout << "Waiting for connection..." << endl;

    sock_peer = accept(sock_listen, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    cout << "PEER CONNECTED!" << endl;
    cout << "Enter '<?q' to quit\nEnter '<?s [filepath]' to send a file" << endl;

    cin.ignore();

    int recvFlag = MSG_DONTWAIT;

    while(true){
        char msg_out[1024] = {0};
        char msg_in[1024] = {0};

        while(true){
            if(recv(sock_peer, msg_in, sizeof(msg_in), recvFlag) > 0){
                cout << "Them: " << msg_in << endl;
            }else{
                recvFlag = MSG_DONTWAIT;
                break;
            }
        }

        if(msg_in[0] == 'q'){
            cout << "PEER DISCONNECTED: quitting" << endl;
            break;
        }

        cout << "You: ";

        string grabinput;

        getline(cin, grabinput);
        strcpy(msg_out, grabinput.c_str());

        if(grabinput.size() == 0){
            //recvFlag = 0;
            cout << "Refreshing..." << endl;
        }else{
            recvFlag = MSG_DONTWAIT;
            if(send(sock_peer, msg_out, sizeof(msg_out), 0) <= 0){
                cout << "send failed!!" << endl;
            }
        }

        if(msg_out[0] == 'q'){
            cout << "quitting..." << endl;
            break;
        }

    }
    
    shutdown(sock_listen, SHUT_RDWR);
    close(sock_listen);
    shutdown(sock_peer, SHUT_RDWR);
    close(sock_peer);
    cout << "DISCONNECTED!" << endl;
    return;
}