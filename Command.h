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

class symbolTable {
private:
    /* Here will be the instance stored. */
    static symbolTable* instance;

    /* Private constructor to prevent instancing. */
    symbolTable();

public:
    static map<string, pair<double, string>> symbol_table_from_simulator;
    static map<string, pair<double, string>> symbol_table_from_text;
    /* Static access method. */
    static symbolTable* getInstance();
};


/*
private:
    //constractor
    symbolTable() {
        //need to complete all table
        symbol_table_from_simulator["\"/instrumentation/airspeed-indicator/indicated-speed-kt\""] = make_pair(0, "airspeed");
        symbol_table_from_simulator["/instrumentation/heading-indicator/offset-deg"] = make_pair(0, "heading");
        symbol_table_from_simulator["/instrumentation/altimeter/indicated-altitude-ft"] = make_pair(0, "altimeter-altitude");
        symbol_table_from_simulator["/instrumentation/altimeter/pressure-alt-ft"] = make_pair(0, "altimeter-pressure");
        symbol_table_from_simulator["/instrumentation/attitude-indicator/indicated-pitch-deg"] = make_pair(0, "attitude-indicated-pitch");
        symbol_table_from_simulator["/instrumentation/attitude-indicator/indicated-roll-deg"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/attitude-indicator/internal-pitch-deg"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/attitude-indicator/internal-roll-deg"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/encoder/indicated-altitude-ft"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/encoder/pressure-alt-ft"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/gps/indicated-altitude-ft"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/gps/indicated-ground-speed-kt\""] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/gps/indicated-vertical-speed"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/heading-indicator/indicated-heading-deg"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/magnetic-compass/indicated-heading-deg"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/slip-skid-ball/indicated-slip-skid"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/turn-indicator/indicated-turn-rate"] = make_pair(0, "");
        symbol_table_from_simulator["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"] = make_pair(0, "");
        symbol_table_from_simulator["/controls/flight/aileron"] = make_pair(0, "");
        symbol_table_from_simulator["/controls/flight/elevator"] = make_pair(0, "");
        symbol_table_from_simulator["/controls/flight/rudder"] = make_pair(0, "");
        symbol_table_from_simulator["/controls/flight/flaps"] = make_pair(0, "");
        symbol_table_from_simulator["/controls/engines/engine/throttle"] = make_pair(0, "");
        symbol_table_from_simulator["/engines/engine/rpm"] = make_pair(0, "");


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
