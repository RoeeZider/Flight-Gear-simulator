//
// Created by roee on 17/12/2019.
//
#include <sys/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include "Command.h"
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <string>
#include <iostream>
#include <arpa/inet.h>


int OpenServerCommand::execute(vector<string> vec) {
    //if expression :
    //else
    int port = stoi(vec[1]);
    stringstream str;
    string s;

    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int client_socket;
    int valread;
    char buffer[1024] = {0};

    //bind socket to IP address
    // we first need to create the sockaddr obj.
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "Could not bind the socket to an IP" << std::endl;
        return -2;
    }
    if (listen(server_fd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr << "Error during listening command" << std::endl;
        return -3;
    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }
    client_socket = accept(server_fd, (struct sockaddr *) &address,
                           (socklen_t *) &addrlen);
    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        return -4;
    }
    valread = read(client_socket, buffer, 1024);
    //when we will end the loop?
    while (valread > 0) {
        s = buffer;
        for (auto it = this->symbol_table_from_simulator.begin();
             it != this->symbol_table_from_simulator.end(); it++) {
            int pos = s.find(",");
            float val = stof(s.substr(0, pos));
            it->second = val;
            s = s.substr(pos + 1);
            cout << it->first + " " << it->second << endl;
            valread = read(client_socket, buffer, 1024);
        }
    }
    close(server_fd);
    //num of jumping
    return 3;
}

int ConnectCommand::execute(vector<string> vec) {
    string temp=vec[1];
    int pos=temp.find(",");
    string ip=temp.substr(1,pos-1);
    const char * ip_c[ip.length()];
    strcpy((char *) ip_c, ip.c_str());
    int port = stoi(temp.substr(pos+1));
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        return -1;
    }

    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr("127.0.0.1");  //the localhost address
    address.sin_port = htons(port);
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.

    // Requesting a connection with the server on local host with port 8081
    int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server" << std::endl;
        return -2;
    } else {
        std::cout << "Client is now connected to server" << std::endl;
    }
    int is_sent=1;
    //if here we made a connection
    while(is_sent!=-1) {
        for (auto it = this->symbol_table_from_text.begin();
             it != this->symbol_table_from_text.end(); it++) {
            if (it->second->getDirection() == 0) {
                char messege[] = "";
                string mes = it->second->getSim() + " ";
                mes += it->second->getValue();
                strcpy((char *) messege, mes.c_str());
                cout<<mes<<endl;
                is_sent = send(client_socket, messege, strlen(messege), 0);
            }
        }
        sleep(1);
    }
    close(client_socket);
    return 3;
}

//הוספה בחמישי בערב
int DefineVarCommand::execute(vector<string> vec) {

    string name = vec[1];
    int dir = 1;
    if (vec[2].compare("<-") != 1) {
        dir = 0;
    }
    string path = vec[4];
    Var *t = new Var(dir, 0, path);
    this->symbol_table_from_text[name] = t;
    return 6;
}
/*
//breaks = 0 כתבתי התחלה לפקודת השמה כמו
int VarCommand::execute(vector<string> vec) {
    for (auto it = symbolTable::symbol_table_from_text.begin(); it != symbolTable::symbol_table_from_text.end(); it++) {
        if (vec[0].compare(it->first) == 0) {
            it->second->setValue(stoi(vec[2]));
            //נבדוק אם צריך לשלוח לסימולטור
            //  if (it->second->getDirection() == 0)

        }
    }

}
*/