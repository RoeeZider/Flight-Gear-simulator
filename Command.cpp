//
// Created by roee on 17/12/2019.
//

#include <sys/socket.h>
#include <bits/socket_type.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include "Command.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unordered_map>
#include <sstream>


int OpenServerCommand::execute(vector<string> vec) {
    int port = stoi(vec[1]);
    stringstream str;
    string s;
    symbolTable *symtble=symbolTable::getInstance();
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int new_socket;
    int valread;
    char buffer[1024] = {0};
    bool flag = true;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr<<"Could not bind the socket to an IP"<<std::endl;
        return -2;
    }
    if (listen(server_fd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr<<"Error during listening command"<<std::endl;
        return -3;
    } else{
        std::cout<<"Server is now listening ..."<<std::endl;
    }
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *) &address,
                            (socklen_t *) &addrlen);
        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        str<<buffer;
        s=str.str();
        for(auto it=symbolTable::symbol_table.begin();it!=symbolTable::symbol_table.end();it++){
            int pos = s.find(",");
            int val = stoi(s.substr(0, pos - 1));
            it->second.first = val;
            s=s.substr(pos+1);
        }

        return 0;
    }

}

