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


class Command {
public:
    virtual int execute(vector<string>) = 0;

    virtual ~Command(){}
};

class OpenServerCommand : public Command {
public:
    int execute(vector<string> vec);
};

class ConnectCommand : public Command {
public:
    int execute(vector<string> vec);
};

class DefineVarCommand : public Command {
public:
    int execute(vector<string> vec);
};

class VarCommand : public Command {
public:
    int execute(vector<string> vec);
};
//הוספה בחמישי בערב
class Var{
private:
    int in1_out0;
    double value;
    string sim;
public:
    Var(int direction, double val,string simu):in1_out0(direction),value(val),sim(simu){};
    int getDirection(){ return this->in1_out0;};
    double getValue(){ return this->value;};
    string getSim(){ return this->sim;};
    void setValue(double new_val){this->value=new_val;};
};

class symbolTable {
private:
    /* Here will be the instance stored. */
    static symbolTable* instance;

    /* Private constructor to prevent instancing. */
    symbolTable(){
        symbol_table_from_simulator["/instrumentation/airspeed-indicator/indicated-speed-kt"] = 0;
        symbol_table_from_simulator["/instrumentation/heading-indicator/offset-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/altimeter/indicated-altitude-ft"] = 0;
        symbol_table_from_simulator["/instrumentation/altimeter/pressure-alt-ft"] =  0;
        symbol_table_from_simulator["/instrumentation/attitude-indicator/indicated-pitch-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/attitude-indicator/indicated-roll-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/attitude-indicator/internal-pitch-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/attitude-indicator/internal-roll-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/encoder/indicated-altitude-ft"] =  0;
        symbol_table_from_simulator["/instrumentation/encoder/pressure-alt-ft"] =  0;
        symbol_table_from_simulator["/instrumentation/gps/indicated-altitude-ft"] =  0;
        symbol_table_from_simulator["/instrumentation/gps/indicated-ground-speed-kt\""] =  0;
        symbol_table_from_simulator["/instrumentation/gps/indicated-vertical-speed"] =  0;
        symbol_table_from_simulator["/instrumentation/heading-indicator/indicated-heading-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/magnetic-compass/indicated-heading-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/slip-skid-ball/indicated-slip-skid"] =  0;
        symbol_table_from_simulator["/instrumentation/turn-indicator/indicated-turn-rate"] =  0;
        symbol_table_from_simulator["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"] =  0;
        symbol_table_from_simulator["/controls/flight/aileron"] =  0;
        symbol_table_from_simulator["/controls/flight/elevator"] =  0;
        symbol_table_from_simulator["/controls/flight/rudder"] =  0;
        symbol_table_from_simulator["/controls/flight/flaps"] =  0;
        symbol_table_from_simulator["/controls/engines/engine/throttle"] =  0;
        symbol_table_from_simulator["/engines/engine/rpm"] = 0;

    };
public:
    static map<string, double> symbol_table_from_simulator;
    static map<string,Var*> symbol_table_from_text;
    /* Static access method. */
    static symbolTable* getInstance();
};



/*
private:
    //constractor
    symbolTable() {
        //need to complete all table
        symbol_table_from_simulator["\"/instrumentation/airspeed-indicator/indicated-speed-kt\""] = 0;
        symbol_table_from_simulator["/instrumentation/heading-indicator/offset-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/altimeter/indicated-altitude-ft"] = 0;
        symbol_table_from_simulator["/instrumentation/altimeter/pressure-alt-ft"] =  0;
        symbol_table_from_simulator["/instrumentation/attitude-indicator/indicated-pitch-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/attitude-indicator/indicated-roll-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/attitude-indicator/internal-pitch-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/attitude-indicator/internal-roll-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/encoder/indicated-altitude-ft"] =  0;
        symbol_table_from_simulator["/instrumentation/encoder/pressure-alt-ft"] =  0;
        symbol_table_from_simulator["/instrumentation/gps/indicated-altitude-ft"] =  0;
        symbol_table_from_simulator["/instrumentation/gps/indicated-ground-speed-kt\""] =  0;
        symbol_table_from_simulator["/instrumentation/gps/indicated-vertical-speed"] =  0;
        symbol_table_from_simulator["/instrumentation/heading-indicator/indicated-heading-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/magnetic-compass/indicated-heading-deg"] =  0;
        symbol_table_from_simulator["/instrumentation/slip-skid-ball/indicated-slip-skid"] =  0;
        symbol_table_from_simulator["/instrumentation/turn-indicator/indicated-turn-rate"] =  0;
        symbol_table_from_simulator["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"] =  0;
        symbol_table_from_simulator["/controls/flight/aileron"] =  0;
        symbol_table_from_simulator["/controls/flight/elevator"] =  0;
        symbol_table_from_simulator["/controls/flight/rudder"] =  0;
        symbol_table_from_simulator["/controls/flight/flaps"] =  0;
        symbol_table_from_simulator["/controls/engines/engine/throttle"] =  0;
        symbol_table_from_simulator["/engines/engine/rpm"] = 0;


    }
public:
    static map<string, pair<double, string>> symbol_table_from_simulator;
    static map<string, pair<double, string>> symbol_table_from_text;
    static symbolTable* instance;
    static symbolTable *getInstance() {
        if (!instance)
            instance=new symbolTable();
        return instance;
    }

};
*/
#endif //FLIGHTGEAR_COMMNAD_H
