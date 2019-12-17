//
// Created by harelfeldman on 12/15/19.
//
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#ifndef FLIGHTGEAR_COMMNAD_H
#define FLIGHTGEAR_COMMNAD_H


class Command {
public:
    virtual int execute(vector<string>) = 0;

    virtual ~Command();
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
public:
    static map<string, pair<double, string>> symbol_table;
    static symbolTable *instance;

    static symbolTable *getInstance() {
        if (!instance)
            instance=new symbolTable();
        return instance;
    };
private:
    symbolTable() {
        symbol_table["airspeed-indicator_indicated-speed-kt"] = make_pair(0, "");
        symbol_table["altimeter_indicated-altitude-ft"] = make_pair(0, "");
        symbol_table["altimeter_pressure-alt-ft"] = make_pair(0, "");
        symbol_table["attitude-indicator_indicated-pitch-deg"] = make_pair(0, "");
        symbol_table["attitude-indicator_indicated-roll-deg"] = make_pair(0, "");
        symbol_table["attitude-indicator_internal-pitch-deg"] = make_pair(0, "");
        symbol_table["attitude-indicator_internal-roll-deg"] = make_pair(0, "");
        symbol_table["encoder_indicated-altitude-ft"] = make_pair(0, "");
        symbol_table["encoder_pressure-alt-ft"] = make_pair(0, "");
        symbol_table["gps_indicated-altitude-ft"] = make_pair(0, "");
        symbol_table["gps_indicated-ground-speed-kt"] = make_pair(0, "");
        symbol_table["gps_indicated-vertical-speed"] = make_pair(0, "");
        symbol_table["indicated-heading-deg"] = make_pair(0, "");
        symbol_table["magnetic-compass_indicated-heading-deg"] = make_pair(0, "");
        symbol_table["slip-skid-ball_indicated-slip-skid"] = make_pair(0, "");
        symbol_table["turn-indicator_indicated-turn-rate"] = make_pair(0, "");
        symbol_table["vertical-speed-indicator_indicated-speed-fpm"] = make_pair(0, "");
        symbol_table["flight_aileron"] = make_pair(0, "");
        symbol_table["flight_elevator"] = make_pair(0, "");
        symbol_table["flight_rudder"] = make_pair(0, "");
        symbol_table["flight_flaps"] = make_pair(0, "");
        symbol_table["engine_throttle"] = make_pair(0, "");
        symbol_table["engine_rpm"] = make_pair(0, "");

    }
};

#endif //FLIGHTGEAR_COMMNAD_H
