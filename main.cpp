#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include "Command.h"
#include "Interpreter.h"

using namespace std;
//declarations
static map<string, double> symbol_table_from_simulator;
static map<string, Var *> symbol_table_from_text;

void lexer(vector<string> &arr, string line);

map<string, Command *> buildMapCommands();

void parser(vector<string> &commands, map<string, Command *> &commandsMap);

void buildMapSimulator();


int main() {
    vector<string> commands;
    map<string, Command *> mapCommands = buildMapCommands();

    buildMapSimulator();

    //change for main argv[1]
    std::ifstream file("fly.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            lexer(commands, line);
        }
        file.close();
    } else {
        cout << "Error in opening file" << endl;
    }

    std::cout << "myvector contains:";
    for (std::vector<string>::iterator it = commands.begin(); it != commands.end(); ++it)
        std::cout << *it + ',' << endl;

    parser(commands, mapCommands);

    return 0;
}
//
void parser(vector<string> &commands, map<string, Command *> &commandsMap) {
    vector<string> temp;
    int index = 0;
    Command *c;
    for (vector<string>::iterator it = commands.begin(); it != commands.end(); ++it) {
        while (*it != "EOL") {
            temp.push_back(*it);
            ++it;
        }
        string operation = commands[index];
        //מוסיף פה משהו ללולאות
        if(operation.compare("while")==0||operation.compare("if")==0){
            while (*it!="}"){
                temp.push_back(*it);
                ++it;
            }
        }
        auto iterator = commandsMap.find(operation);
        if (iterator != commandsMap.end()) {
            c = commandsMap.at(operation);
        } else {
            c = commandsMap.at("var");
        }
        // c== &commandsMap.at(commands[index]);
        if (c != NULL) {
            //what if we are waiting to the number?
            index += c->execute(temp);
        }
        //no memory allocating
        temp.resize(0);
    }
}

void lexer(vector<string> &arr, string line) {
    int j = 0;
    string word;
    string word2;
    bool flag = false;
    vector<string> vec2;
    static map<string, double> symbol_table_from_simulator;
    static map<string, Var *> symbol_table_from_text;
    //save the firat word from the line
    for (int i = 0; i < line.length(); i++) {
        while (line[i] != ' ' && line[i] != '(' && i < line.length()) {
            if (line[i] != '\t') {
                word = word + line[i];
            }
            i++;
        }
        break;
    }

    if ((word.compare("Print") == 0) || (word.compare("Sleep") == 0)) {
        arr.push_back(word);
        for (int i = 5; i < line.length(); i++) {
            if (line[i] == '(') {
                while (i < line.length() - 2) {
                    i++;
                    word2 = word2 + line[i];

                }
            }
        }

        arr.push_back(word2);
        arr.push_back("EOL");
        word2 = "";
    }

//check 'if' case

    else if ((word.compare("while") == 0) || (word.compare("if") == 0)) {
        arr.push_back(word);
        for (int i = word.length() + 1; i < line.length(); i++) {
            while (line[i] != '{') {
                //check spaces
                word2 = word2 + line[i];
                i++;
            }
        }
        word2 = word2.substr(0, word2.length() - 1);
        arr.push_back(word2);
        arr.push_back("{");
        arr.push_back("EOL");
        word2 = "";
    } else if (word.compare("var") == 0) {
        arr.push_back("var");
        int i = 4;
        while (i != line.length()) {
            if (line[i] != ' ' && line[i] != '(' && line[i] != ')' && line[i] != '\n' && line[i] != '\t' &&
                line[i] != '-' && line[i] != '>' && line[i] != '<' && line[i] != '=') {
                word2 = word2 + line[i];
                i++;
            } else {
                if (!word2.compare("") == 0) {
                    arr.push_back(word2);
                    word2 = "";
                }
                if (line[i] == '-') {
                    if (line[i + 1] == '>') {
                        arr.push_back("->");
                    }
                }
                if (line[i] == '<') {
                    if (line[i + 1] == '-') {
                        arr.push_back("<-");
                    }
                }
                if (line[i] == '=') {
                    arr.push_back("=");
                    word2 = "";
                    flag = true;
                    i++;
                    break;
                }

                i++;
            }
        }
        if (flag) {
            while (i != line.length()) {
                if (line[i] != '\n' && line[i] != '\t') {
                    word2 = word2 + line[i];
                }
                i++;
            }
            if (word2[0] == ' ') {
                word2 = word2.substr(1);
            }
            arr.push_back(word2);
        }
        arr.push_back("EOL");
    } else {

        int i = 0;
        while (i != line.length()) {
            if (line[i] != '\n' && line[i] != '\t' && line[i] != '=' && line[i] != ' ') {
                word2 = word2 + line[i];
                i++;
            } else {
                if (!word2.compare("") == 0) {
                    arr.push_back(word2);
                    word2 = "";
                }
                if (line[i] == '=') {
                    arr.push_back("=");
                    word2 = "";
                    flag = true;
                    i++;
                    break;
                }
                i++;
            }
        }
        if (!word2.compare("") == 0) {
            arr.push_back(word2);
        }
        if (flag) {
            while (i != line.length()) {
                if (line[i] != '\n' && line[i] != '\t') {
                    word2 = word2 + line[i];
                }
                i++;
            }
            if (word2[0] == ' ') {
                word2 = word2.substr(1);
            }
            arr.push_back(word2);
        }

        arr.push_back("EOL");

    }
}


map<string, Command *> buildMapCommands() {
    map<string, Command *> my_map;
    my_map.insert(make_pair("openDataServer", new OpenServerCommand(symbol_table_from_simulator)));
    my_map.insert(make_pair("connectControlClient", new ConnectCommand(symbol_table_from_text)));
    my_map.insert(make_pair("var", new DefineVarCommand(symbol_table_from_text,symbol_table_from_simulator)));
    my_map.insert(make_pair("Print", new PrintCommand(symbol_table_from_text)));
    my_map.insert(make_pair("Sleep", new SleepCommand()));
    my_map.insert(make_pair("while", new ConditionCommand(symbol_table_from_text,symbol_table_from_simulator)));
    my_map.insert(make_pair("if", new ConditionCommand(symbol_table_from_text,symbol_table_from_simulator)));
    return my_map;
}

void buildMapSimulator() {
    symbol_table_from_simulator["/instrumentation/airspeed-indicator/indicated-speed-kt"] = 0;
    symbol_table_from_simulator["sim/time/warp"]=0;
    symbol_table_from_simulator["controls/switches/magnetos"]=0;
    symbol_table_from_simulator["/instrumentation/heading-indicator/offset-deg"] = 0;
    symbol_table_from_simulator["/instrumentation/altimeter/indicated-altitude-ft"] = 0;
    symbol_table_from_simulator["/instrumentation/altimeter/pressure-alt-ft"] = 0;
    symbol_table_from_simulator["/instrumentation/attitude-indicator/indicated-pitch-deg"] = 0;
    symbol_table_from_simulator["/instrumentation/attitude-indicator/indicated-roll-deg"] = 0;
    symbol_table_from_simulator["/instrumentation/attitude-indicator/internal-pitch-deg"] = 0;
    symbol_table_from_simulator["/instrumentation/attitude-indicator/internal-roll-deg"] = 0;
    symbol_table_from_simulator["/instrumentation/encoder/indicated-altitude-ft"] = 0;
    symbol_table_from_simulator["/instrumentation/encoder/pressure-alt-ft"] = 0;
    symbol_table_from_simulator["/instrumentation/gps/indicated-altitude-ft"] = 0;
    symbol_table_from_simulator["/instrumentation/gps/indicated-ground-speed-kt\""] = 0;
    symbol_table_from_simulator["/instrumentation/gps/indicated-vertical-speed"] = 0;
    symbol_table_from_simulator["/instrumentation/heading-indicator/indicated-heading-deg"] = 0;
    symbol_table_from_simulator["/instrumentation/magnetic-compass/indicated-heading-deg"] = 0;
    symbol_table_from_simulator["/instrumentation/slip-skid-ball/indicated-slip-skid"] = 0;
    symbol_table_from_simulator["/instrumentation/turn-indicator/indicated-turn-rate"] = 0;
    symbol_table_from_simulator["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"] = 0;
    symbol_table_from_simulator["/controls/flight/aileron"] = 0;
    symbol_table_from_simulator["/controls/flight/elevator"] = 0;
    symbol_table_from_simulator["/controls/flight/rudder"] = 0;
    symbol_table_from_simulator["/controls/flight/flaps"] = 0;
    symbol_table_from_simulator["/controls/engines/engine/throttle"] = 0;
    symbol_table_from_simulator["controls/engines/current-engine/throttle"]=0;
    symbol_table_from_simulator["controls/switches/master-avionics"]=0;
    symbol_table_from_simulator["controls/switches/starter"]=0;
    symbol_table_from_simulator["engines/active-engine/auto-start"]=0;
    symbol_table_from_simulator["controls/flight/speedbrake"]=0;
    symbol_table_from_simulator["sim/model/c172p/brake-parking"]=0;
    symbol_table_from_simulator["controls/engines/engine/primer"]=0;
    symbol_table_from_simulator["controls/engines/current-engine/mixture"]=0;
    symbol_table_from_simulator["controls/switches/master-bat"]=0;
    symbol_table_from_simulator["controls/switches/master-alt"]=0;
    symbol_table_from_simulator["/engines/engine/rpm"] = 0;

}
