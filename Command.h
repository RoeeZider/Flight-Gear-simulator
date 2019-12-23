//
// Created by harelfeldman on 12/15/19.
//
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Expression.h"
#include <queue>
#include <list>
#include <thread>

using namespace std;

#ifndef FLIGHTGEAR_COMMNAD_H
#define FLIGHTGEAR_COMMNAD_H

//הוספה בחמישי בערב
class Var {
private:
    int in1_out0;
    double value;
    string sim;
    int sent;
public:
    map<string, double> &symbol_table_from_simulator;

    Var(int direction, double val, string simu, map<string, double> &symbol_table) : symbol_table_from_simulator(
            symbol_table) {
        in1_out0 = direction;
        value = val;
        sim = simu;
        sent = 0;
    };

    int getDirection() { return this->in1_out0; };

    double getValue() {
        if (this->in1_out0 == 1) {
            auto it = symbol_table_from_simulator.find(sim);
            this->value = it->second;
        }
        return this->value;
    };

    string getSim() { return this->sim; };

    void setValue(double new_val) {
        this->value = new_val;
        if (this->getDirection() == 0)
            this->sent = 0;
    };

    void setSent() {
        this->sent = 1;
    };//

    int getSent() {
        return this->sent;
    };
};

class Command {
public:
    virtual int execute(vector<string>) = 0;

    virtual ~Command() {}
};

//ui
class OpenServerCommand : public Command {
public:
    map<string, double> &symbol_table_from_simulator;

    OpenServerCommand(map<string, double> &symbol_table) : symbol_table_from_simulator(symbol_table) {};

    int execute(vector<string> vec);
    static void  readFromSimulator(int client_socket,map<string, double> symbol_table_from_simulator);
      //void readFromSimulator(int client_socket);
};

class ConnectCommand : public Command {
public:

    map<string, Var *> &symbol_table_from_text;

    ConnectCommand(map<string, Var *> &symbol_table) : symbol_table_from_text(symbol_table) {};

    int execute(vector<string> vec);

    static void  readFromText(int client_socket,map<string, double>  symbol_table_from_text);
};

class DefineVarCommand : public Command {
public:
    map<string, Var *> &symbol_table_from_text;
    map<string, double> &symbol_table_from_simulator;

    DefineVarCommand(map<string, Var *> &symbol_table, map<string, double> &from_simulator) :
            symbol_table_from_text(symbol_table),
            symbol_table_from_simulator(from_simulator) {};

    int execute(vector<string> vec);
};

class PrintCommand : public Command {
public:
    map<string, Var *> &symbol_table_from_text;

    PrintCommand(map<string, Var *> &symbol_table) : symbol_table_from_text(symbol_table) {};

    int execute(vector<string> vec);
};

class SleepCommand : public Command {
public:

    int execute(vector<string> vec);
};

class ConditionCommand : public Command {
public:
    map<string, Var *> &symbol_table_from_text;
    map<string, double> &symbol_table_from_simulator;


 //  ConditionCommand(map<string, Var *> &symbol_table, map<string, double> &from_simulator):
   //         symbol_table_from_text(symbol_table),
     //       symbol_table_from_simulator(from_simulator) {};

    int execute(vector<string> vec);
};

#endif //FLIGHTGEAR_COMMNAD_H
