#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "Command.h"

using namespace std;

void lexer(vector<string> &arr, string line);


int main() {
    vector<string> commands;
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
        std::cout << ' ' << *it;
    std::cout << '\n';

    return 0;
}

void Parser(vector<string> commands, map<string, Command> commandsMap) {
    vector<string> temp;
    int index = 0;
    for (vector<string>::iterator it = commands.begin(); it != commands.end(); ++it) {
        while (*it != "EOL") {
            temp.push_back(*it);
            ++it;
        }
        auto ite = commandsMap.find(commands[index]);
        Command *c = ite->second;
        if (c != NULL) {
            index += c->execute(temp);
        }
    }
}

void lexer(vector<string> &arr, string line) {
    int j = 0;
    string word;
    string word2;
    vector<string> vec2;
    for (int i = 0; i < line.length(); i++) {
        while (line[i] != ' ' && line[i] != '(') {
            word = word + line[i];
            i++;
        }
        break;
    }

    if (word.compare("Print") == 0) {
        for (int i = 5; i < line.length(); i++) {
            if (line[i] == '(') {
                while (i < line.length() - 2) {
                    i++;
                    word2 = word2 + line[i];

                }
            }
        }
        arr.push_back("Print");
        arr.push_back(word2);
        arr.push_back("EOL\n");
        word2 = "";
    }
        //maybe in the regular case


    else if (word.compare("while") == 0) {
        for (int i = 6; i < line.length(); i++) {
            while (line[i] != '{') {
                //check spaces
                word2 = word2 + line[i];
                i++;
            }
        }
        arr.push_back("while");
        word2 = word2.substr(0, word2.length() - 1);
        arr.push_back(word2);
        arr.push_back("{");
        arr.push_back("EOL\n");
        word2 = "";
    } else {
        for (int i = 0; i < line.length(); i++) {
            while (line[i] != ' ' && line[i] != '(' && line[i] != ')' && line[i] != '\n') {
                if (i >= line.length()) {
                    break;
                }
                word2 = word2 + line[i];
                i++;
            }
            arr.push_back(word2);
            word2 = "";
        }
        arr.push_back("EOL");
    }
}

void build_parameters_map() {
    symbol_table["airspeed-indicator_indicated-speed-kt"] = make_pair(0,"");
    symbol_table["altimeter_indicated-altitude-ft"] = make_pair(0,"");
    symbol_table["altimeter_pressure-alt-ft"] = make_pair(0,"");
    symbol_table["attitude-indicator_indicated-pitch-deg"] = make_pair(0,"");
    symbol_table["attitude-indicator_indicated-roll-deg"] = make_pair(0,"");
    symbol_table["attitude-indicator_internal-pitch-deg"] = make_pair(0,"");
    symbol_table["attitude-indicator_internal-roll-deg"] = make_pair(0,"");
    symbol_table["encoder_indicated-altitude-ft"] = make_pair(0,"");
    symbol_table["encoder_pressure-alt-ft"] = make_pair(0,"");
    symbol_table["gps_indicated-altitude-ft"] = make_pair(0,"");
    symbol_table["gps_indicated-ground-speed-kt"] = make_pair(0,"");
    symbol_table["gps_indicated-vertical-speed"] = make_pair(0,"");
    symbol_table["indicated-heading-deg"] = make_pair(0,"");
    symbol_table["magnetic-compass_indicated-heading-deg"] = make_pair(0,"");
    symbol_table["slip-skid-ball_indicated-slip-skid"] = make_pair(0,"");
    symbol_table["turn-indicator_indicated-turn-rate"] = make_pair(0,"");
    symbol_table["vertical-speed-indicator_indicated-speed-fpm"] = make_pair(0,"");
    symbol_table["flight_aileron"] = make_pair(0,"");
    symbol_table["flight_elevator"] = make_pair(0,"");
    symbol_table["flight_rudder"] = make_pair(0,"");
    symbol_table["flight_flaps"] = make_pair(0,"");
    symbol_table["engine_throttle"] = make_pair(0,"");
    symbol_table["engine_rpm"] = make_pair(0,"");
}