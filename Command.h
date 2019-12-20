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

using namespace std;

#ifndef FLIGHTGEAR_COMMNAD_H
#define FLIGHTGEAR_COMMNAD_H

//הוספה בחמישי בערב
class Var {
private:
    int in1_out0;
    double value;
    string sim;
public:
    Var(int direction, double val, string simu) : in1_out0(direction), value(val), sim(simu) {};

    int getDirection() { return this->in1_out0; };

    double getValue() { return this->value; };

    string getSim() { return this->sim; };

    void setValue(double new_val) { this->value = new_val; };
};

class Command {
public:
    virtual int execute(vector<string>) = 0;

    virtual ~Command() {}
};

class OpenServerCommand : public Command {
public:
    map<string, double> symbol_table_from_simulator;

    OpenServerCommand(map<string, double> &symbol_table) : symbol_table_from_simulator(symbol_table) {};

    int execute(vector<string> vec);

};

class ConnectCommand : public Command {
public:
    map<string, double> symbol_table_from_simulator;
    map<string, Var *> symbol_table_from_text;

    ConnectCommand(map<string, double> &from_simulator, map<string, Var *> &from_text) {
        this->symbol_table_from_simulator = from_simulator;
        this->symbol_table_from_text = from_text;
    };

    int execute(vector<string> vec);
};

class DefineVarCommand : public Command {
public:
    map<string, Var *> symbol_table_from_text;

    DefineVarCommand(map<string, Var *> &symbol_table) {
        this->symbol_table_from_text = symbol_table;
    };

    int execute(vector<string> vec);
};

class VarCommand : public Command {
public:
    // VarCommand(map<string, double> &symbol_table_from_text);
    int execute(vector<string> vec);
};

#endif //FLIGHTGEAR_COMMNAD_H
