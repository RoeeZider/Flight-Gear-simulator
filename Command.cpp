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
#include "Interpreter.h"
#include <thread>
 void OpenServerCommand::readFromSimulator(int client_socket, map<string, double> symbol_table_from_simulator) {
    int valread;
    string s;
    char buffer[1024] = {0};
    valread = read(client_socket, buffer, 1024);
    //when we will end the loop?
    while (valread > 0) {
        s = buffer;
        for (auto it = symbol_table_from_simulator.begin();
             it != symbol_table_from_simulator.end(); it++) {
            int pos = s.find(",");
            float val = stof(s.substr(0, pos));
            it->second = val;
            s = s.substr(pos + 1);
            cout << it->first + " " << it->second << endl;
            valread = read(client_socket, buffer, 1024);
        }
    }
}
void ConnectCommand::readFromText(int client_socket, map<string, double> symbol_table_from_text) {
    while (symbol_table_from_text.find("end_client")->second->getDirection() != 5) {
        for (auto it = symbol_table_from_text.begin();
             it != symbol_table_from_text.end(); ++it) {
            if (it->second->getDirection() == 0 && it->second->getSent() == 0) {
                it->second->setSent();
                char messege[] = "";
                string mes = "set " + it->second->getSim().substr(2, it->second->getSim().length() - 3) + " ";
                mes += to_string(it->second->getValue());
                mes += "\r\n";
                strcpy(messege, mes.c_str());
                cout << mes << endl;
                int is_sent = send(client_socket, messege, strlen(messege), 0);
            }
        }
        //sleep(1);
    }
}

int OpenServerCommand::execute(vector<string> vec) {
    //if expression :
    //else

    int port = stoi(vec[1]);
    stringstream str;


    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int client_socket;


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
     thread t1(readFromSimulator,client_socket,symbol_table_from_simulator);
    t1.detach();
    //maybe just if done
    close(client_socket);
    close(server_fd);
    //num of jumping
    return 3;
}



int ConnectCommand::execute(vector<string> vec) {
    string temp = vec[1];
    int pos = temp.find(",");
    string ip = temp.substr(1, pos - 1);
    const char *ip_c[ip.length()];
    strcpy((char *) ip_c, ip.c_str());
    int port = stoi(temp.substr(pos + 1));
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
        char messege[] = "set controls/flight/rudder -1\r\n";
        int is_sent = send(client_socket, messege, strlen(messege), 0);
    }
    //if here we made a connection
    //need to change it to the end of the file
    thread t2(readFromText,client_socket,symbol_table_from_text);
    t2.detach();
    close(client_socket);
    return 3;
}



//הוספה בחמישי בערב
int DefineVarCommand::execute(vector<string> vec) {
    Interpreter* interpeter=new Interpreter;
    int dir = 1;
    if (vec[0].compare("var") == 0) {
        string name = vec[1];

        //there is a case og '=' - initialize a different var to
        if (vec[2].compare("=") == 0) {
//            Expression* exp=interpreter.interpret(vec[3]);
            if (isdigit(atoi(vec[3].c_str()))) {
                this->symbol_table_from_text[vec[1]] = new Var(2, stod(vec[3]), "",symbol_table_from_simulator);
            } else {
                Var *v = this->symbol_table_from_text.at(vec[3]);
                this->symbol_table_from_text[vec[1]] = new Var(2, v->getValue(), "",symbol_table_from_simulator);
            }
        } else {
            if (vec[2].compare("->") == 0) {
                dir = 0;
            }


            string path = vec[3];
            Var *t = new Var(dir, 0, path,symbol_table_from_simulator);
            this->symbol_table_from_text[name] = t;
        }
    } else {
        //maybe vec[1] is expression
        double d = stod(vec[2]);
        this->symbol_table_from_text[vec[0]]->setValue(d);
        //send to simulator the new value
    }

    return vec.size() + 1;
}

int PrintCommand::execute(vector<string> vec) {
    string printLine = vec[1];
    auto it = symbol_table_from_text.find(printLine);
    if (it != symbol_table_from_text.end()) {
        cout << it->second->getValue() << endl;
    }
    //clear the """
    printLine = printLine.substr(1, printLine.length() - 1);
    cout << printLine << endl;
    return 3;
}

int SleepCommand::execute(vector<string> vec) {
    sleep(stod((vec[1])));
    return 3;
}
