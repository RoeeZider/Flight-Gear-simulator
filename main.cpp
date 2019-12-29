#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <algorithm>
#include "Command.h"
#include "Interpreter.h"

using namespace std;
//declarations
static map<string, double> symbol_table_from_simulator;
static map<string, Var *> symbol_table_from_text;
static queue<string> commandsQueue;
void lexer(vector<string> &arr, string line);

void buildMapCommands();

void clearSpaces(string &word);

void parser(vector<string> &commands, map<string, Command *> &commandsMap);

void buildMapSimulator();

static vector<string> commands;
static map<string, Command *> mapCommands;

int main() {

    buildMapCommands();
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
    commandsQueue.push("end_client");
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
        //try
        if (temp.size() != 0) {
            string operation = commands[index];
            //מוסיף פה משהו ללולאות
            if (operation.compare("while") == 0 || operation.compare("if") == 0) {
                while (*it != "}") {
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

}

void lexer(vector<string> &arr, string line) {
    replace(line.begin(), line.end(), '\t', ' ');
    clearSpaces(line);
    clearSpaces(line);
    clearSpaces(line);
    string word;
    string word2;
    clearSpaces(line);
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
    clearSpaces(word);
    if ((word.compare("Print") == 0) || (word.compare("Sleep") == 0)) {
        arr.push_back(word);

        word2 = line.substr(line.find('(') + 1);
        word2 = word2.substr(0, word2.length() - 1);

        arr.push_back(word2);
        arr.push_back("EOL");
        word2 = "";
    } else if ((word.compare("while") == 0) || (word.compare("if") == 0)) {
        arr.push_back(word);

        word2 = line.substr(word.length() + 1);
        word2 = word2.substr(0, word2.length() - 1);
        arr.push_back(word2);
        arr.push_back("{");
        arr.push_back("EOL");
        word2 = "";

    } else if (word.compare("var") == 0) {
        arr.push_back(word);
        word2 = line.substr(4);
        int token = line.find('=');
        string temp;
        if (line.find('=') == -1) {
            int x = word2.find('>');
            int y = word2.find('<');
            if (x > y) {
                temp = word2.substr(0, x - 2);
                arr.push_back(temp);
                arr.push_back("->");
                word2 = word2.substr(x + 1);
            } else {
                temp = word2.substr(0, y - 1);
                arr.push_back(temp);
                arr.push_back("<-");
                word2 = word2.substr(y + 1);
            }
            word2 = word2.substr(word2.find('(') + 1);
            word2 = word2.substr(1, word2.length() - 3);
            arr.push_back(word2);

        } else {
            string temp;
            temp = word2.substr(0, word2.find('='));
            clearSpaces(temp);
            arr.push_back(temp);
            arr.push_back("=");
            word2 = word2.substr(word2.find('=') +1);
            clearSpaces(word2);
            arr.push_back(word2);

        }
        arr.push_back("EOL");
    } else {
        if (line.find('=') == -1) {
            if (word.compare("}") != 0) {
                arr.push_back(word);
                word2 = line.substr(line.find('(') + 1);
                word2 = word2.substr(0, word2.length() - 1);
                clearSpaces(word2);
                arr.push_back(word2);
            } else {
                arr.push_back("}");
            }
        } else {
            string temp;
            temp = line.substr(0, line.find('='));
            clearSpaces(temp);
            arr.push_back(temp);
            arr.push_back("=");
            word2 = line.substr(line.find('=') + 1);
            clearSpaces(word2);
            arr.push_back(word2);
        }
        arr.push_back("EOL");

    }


}


void buildMapCommands() {
    // map<string, Command *> my_map;
    mapCommands.insert(make_pair("openDataServer", new OpenServerCommand(symbol_table_from_simulator,symbol_table_from_text)));
    mapCommands.insert(make_pair("connectControlClient", new ConnectCommand(commandsQueue)));
    mapCommands.insert(make_pair("var", new DefineVarCommand(symbol_table_from_text, symbol_table_from_simulator,commandsQueue)));
    mapCommands.insert(make_pair("Print", new PrintCommand(symbol_table_from_text)));
    mapCommands.insert(make_pair("Sleep", new SleepCommand(symbol_table_from_text)));
    mapCommands.insert(make_pair("while", new ConditionCommand(symbol_table_from_text, symbol_table_from_simulator,mapCommands,
                                                               reinterpret_cast<void (*)(vector<std::string>,
                                                                                         map<std::string, struct Command *>)>(&parser))));
    mapCommands.insert(make_pair("if", new ConditionCommand(symbol_table_from_text, symbol_table_from_simulator,mapCommands,
                                                            reinterpret_cast<void (*)(vector<std::string>,
                                                                                      map<std::string, struct Command *>)>(&parser))));
    // return my_map;
}

void buildMapSimulator() {
    symbol_table_from_simulator["/instrumentation/airspeed-indicator/indicated-speed-kt"] = 0;
    symbol_table_from_simulator["/sim/time/warp"] = 0;
    symbol_table_from_simulator["/controls/switches/magnetos"] = 0;
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
    symbol_table_from_simulator["/instrumentation/gps/indicated-ground-speed-kt"] = 0;
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
    symbol_table_from_simulator["/controls/engines/current-engine/throttle"] = 0;
    symbol_table_from_simulator["/controls/switches/master-avionics"] = 0;
    symbol_table_from_simulator["/controls/switches/starter"] = 0;
    symbol_table_from_simulator["/engines/active-engine/auto-start"] = 0;
    symbol_table_from_simulator["/controls/flight/speedbrake"] = 0;
    symbol_table_from_simulator["/sim/model/c172p/brake-parking"] = 0;
    symbol_table_from_simulator["/controls/engines/engine/primer"] = 0;
    symbol_table_from_simulator["/controls/engines/current-engine/mixture"] = 0;
    symbol_table_from_simulator["/controls/switches/master-bat"] = 0;
    symbol_table_from_simulator["/controls/switches/master-alt"] = 0;
    symbol_table_from_simulator["/engines/engine/rpm"] = 0;

}

void clearSpaces(string &word) {
    //remove space instead
    if (word[0] == ' ') {
        word = word.substr(1);
    }
    if (word[word.length() - 1] == ' ') {
        word = word.substr(0, word.length() - 1);
    }
}
