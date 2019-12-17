//
// Created by harelfeldman on 12/15/19.
//
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#ifndef FLIGHTGEAR_COMMNAD_H
#define FLIGHTGEAR_COMMNAD_H


class Command {
public:
    virtual int execute(vector<string>)=0;
    virtual ~Command();
};
class OpenServerCommand:public Command{
public:
    int execute(vector<string> vec)();
};
class ConnectCommand:public Command{
public:
    int execute(vector<string> vec)();
};

class DefineVarCommand:public Command{
public:
    int execute(vector<string> vec)();
};

#endif //FLIGHTGEAR_COMMNAD_H
