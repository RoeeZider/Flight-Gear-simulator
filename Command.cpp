//
// Created by roee on 17/12/2019.
//

#include <sys/socket.h>

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
    symbolTable *table = symbolTable::getInstance();
    while (valread > 0) {
        //printf("%s\n", buffer);
        str << buffer;
        s = str.str();
        for (auto it = table->symbol_table_from_simulator.begin();
             it != table->symbol_table_from_simulator.end(); it++) {
            int pos = s.find(",");
            int val = stoi(s.substr(0, pos - 1));
            it->second = val;
            s = s.substr(pos + 1);
            valread = read(client_socket, buffer, 1024);
        }
    }
    close(server_fd);
    //num of jumping
    return 3;
}

int ConnectCommand::execute(vector<string> vec) {
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
    symbolTable::symbol_table_from_text[name] = t;
    return 6;
}

//breaks = 0 כתבתי התחלה לפקודת השמה כמו
int VarCommand::execute(vector<string> vec) {
    for (auto it = symbolTable::symbol_table_from_text.begin(); it != symbolTable::symbol_table_from_text.end(); it++) {
        if (vec[0].compare(it->first) == 1) {
            it->second->setValue(stoi(vec[2]));
            //נבדוק אם צריך לשלוח לסימולטור
            if (it->second->getDirection() == 0) {
                //לשלוח דרך הקליינט "sim
            }
        }
    }

}