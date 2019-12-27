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
#include <mutex>

double Var::getValue() {
    if (this->in1_out0 == 1) {
        double it = symbol_table_from_simulator.at(sim);
        //   cout<<it<<" at get value"<<endl;
        this->value = it;
    }
    return this->value;
}

void OpenServerCommand::readFromSimulator(int client_socket, map<string, double> &symbol_table_from_simulator) {
   // std::this_thread::sleep_for(chrono::milliseconds((int) 60000));
    // cout << symbol_table_from_simulator.size() << endl;
    int valread;
    //cout << "bi   " << client_socket << endl;
    string s;
    mutex m;
    char buffer[1024] = {0};
    valread = read(client_socket, buffer, 1024);
    // cout << valread << endl;
    //cout << "in th" << buffer << endl;
    int j=0;
    int k=0;
    string newstr;
    //when we will end the loop?
    s = buffer;
    while (s != " ") {
        newstr = "";

        while (s[k] != '\n') {
            k++;
        }
        j = k + 1;
        while (s[j] != '\n') {
            newstr += s[j];
            j++;
        }

        m.lock();
        if (newstr.size() > 250) {
            cout << "read from simulator:" << endl;
            for (auto it = symbol_table_from_simulator.begin();
                 it != symbol_table_from_simulator.end(); ++it) {
                cout << newstr << endl;
                int pos = newstr.find(",");
                float val = stof(newstr.substr(0, pos));
                it->second = val;
                newstr = newstr.substr(pos + 1);
                cout << it->first + " " << it->second << endl;

            }

        }
        valread = read(client_socket, buffer, 1024);
        s = buffer;
    }
    std::this_thread::sleep_for(chrono::milliseconds((int) 60000));
    m.unlock();

    close(client_socket);
//dont forget close socket

}

void ConnectCommand::readFromText(int client_socket, map<string, Var *> &symbol_table_from_text) {
    cout << "inside client thread " << endl;
    mutex m;
    //cout << symbol_table_from_text.find("end_client")->second->getValue() << endl;
    while (symbol_table_from_text.find("end_client")->second->getValue() != 5) {
        m.lock();
        for (auto it = symbol_table_from_text.begin();
             it != symbol_table_from_text.end(); ++it) {
            if (it->second->getDirection() == 0 && it->second->getSent() == 0) {
                it->second->setSent();
                char messege[] = "";
                string mes = "set " + it->second->getSim() + " " + to_string(it->second->getValue()) + "\r\n";

                // mes += mes+"\r\n";

                cout << mes << endl;
                //   this_thread::sleep_for(chrono::seconds(1));
                //  messege[strlen(messege)] = '\r\n';
                int is_sent = send(client_socket, mes.c_str(), strlen(messege), 0);
            }
        }
        m.unlock();
        //  sleep(2);
    }
}

int OpenServerCommand::execute(vector<string> vec) {
    //if expression :
    //else
    Interpreter *interpreter = new Interpreter(symbol_table_from_text);
    Expression *e = interpreter->interpret(vec[1]);
    double p = e->calculate();
    int port = (int) p;
    stringstream str;
    char buffer[1024] = {0};

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
    std::cout << "Server is coonected" << endl;
    thread t1(readFromSimulator, client_socket, ref(symbol_table_from_simulator));
    t1.detach();
    //maybe just if done

    //num of jumping
    return 3;
}


int ConnectCommand::execute(vector<string> vec) {
    string temp = vec[1];
    int pos = temp.find(",");
    string ip = temp.substr(1, pos - 1);
    Interpreter *interpreter = new Interpreter(symbol_table_from_text);
    const char *ip_c[ip.length()];
    Expression *e = interpreter->interpret(temp.substr(pos + 1));
    double p = e->calculate();
    strcpy((char *) ip_c, ip.c_str());
    int port = (int) p;
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
    thread t2(readFromText, client_socket, ref(symbol_table_from_text));
    t2.detach();
    //  close(client_socket);
    return 3;
}


//הוספה בחמישי בערב
int DefineVarCommand::execute(vector<string> vec) {
    Interpreter *interpreter = new Interpreter(symbol_table_from_text);

    int dir = 1;
    if (vec[0].compare("var") == 0) {
        string name = vec[1];

        //there is a case og '=' - initialize a different var to
        if (vec[2].compare("=") == 0) {
            Expression *e = interpreter->interpret(vec[3]);
            this->symbol_table_from_text[vec[1]] = new Var(2, e->calculate(), "", symbol_table_from_simulator);

        } else {
            if (vec[2].compare("->") == 0) {
                dir = 0;
            }
            string path = vec[3];
            Var *t = new Var(dir, 0, path, symbol_table_from_simulator);
            this->symbol_table_from_text[name] = t;
        }
    } else {
        //maybe vec[1] is expression
        Expression *e = interpreter->interpret(vec[2]);
        this->symbol_table_from_text[vec[0]]->setValue(e->calculate());
        //send to simulator the new value
    }

    return vec.size() + 1;
}

int PrintCommand::execute(vector<string> vec) {
    string printLine = vec[1];
    if (vec[1][0] != 34) {//34 is the char " in ascii
        Interpreter *i = new Interpreter(symbol_table_from_text);
        Expression *e = i->interpret(vec[1]);
        cout << e->calculate() << endl;
    }
        //clear the """
    else {
        printLine = printLine.substr(1, printLine.length() - 1);
        cout << printLine << endl;
    }
    return 3;
}

int SleepCommand::execute(vector<string> vec) {
    Interpreter *interpreter = new Interpreter(symbol_table_from_text);
    Expression *e = interpreter->interpret(vec[1]);
    std::this_thread::sleep_for(chrono::milliseconds((int) e->calculate()));
    return 3;
}

int ConditionCommand::execute(vector<string> vec) {
    vector<string> vec1;
    Interpreter *interpreter = new Interpreter(symbol_table_from_text);
    int i = 0;
    double val = 0;
    char sign;
    int flag1 = 0, flag2 = 1, flag3 = 0, flag4 = 1;
    string temp = "";
    while (vec[1][i] > 64 || vec[1][i] < 60) {
        if (vec[1][i] != ' ')
            temp += vec[1][i];
        i++;
    }
    string condition_var = temp;
    if (condition_var.compare("rpm") == 0) {

    }
    auto it = symbol_table_from_text.find(condition_var);
    Var *vari = it->second;
    double value = vari->getValue();
    //cout << val << " זהו הערך של rpm" << endl;
    sign = vec[1][i];
    i++;
    //אם הסימן הוא >  פלאג2 הוא -1
    if (sign == '<')
        flag2 = -1;
    if (sign == '=')
        flag4 = 0;
    if (vec[1][i] == '=') {
        flag1 = 1;
        i++;
    }
    temp = "";
    while (i < vec[1].length()) {
        if (vec[1][i] != ' ')
            temp += vec[1][i];
        i++;
    }
    Expression *e = interpreter->interpret(temp);
    val = e->calculate();
    for (auto it = vec.begin() + 4; it != vec.end(); ++it) {
        vec1.push_back(*it);
    }
    if (vec[0].compare("while") == 0) {
        while (flag4 * ((value - val) * flag2 > 0) || (flag1 == 1 && ((value - val) * flag2 >= 0)) ||
               (flag4 == 0 && value == val)) {
            //כאשר הסימן הוא == דגל4 הוא 0 ולכן התנאי הראשון אינו נכון וייבדק רק השני.
            // כאשר הסימן הוא > דגל2 הוא 1- ולכן כשנכפיל בהפרש,נקבל אמת רק אם השני גדול מהראשון.
            //כאשר קיבלנו => או =< נוסיף את דגל1
            parser(vec1, mapCommand);
            value = symbol_table_from_text.find(condition_var)->second->getValue();
            e = interpreter->interpret(temp);
            val = e->calculate();
        }
    } else if (vec[0].compare("if") == 0) {
        if (flag4 * ((value - val) * flag2 > 0) || (flag1 == 1 && ((value - val) * flag2 >= 0)) ||
            (flag4 == 0 && value == val)) {
            parser(vec1, mapCommand);
        }
    }

    return vec.size() + 2;

}