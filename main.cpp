#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

static map<string, double> parameters;

void lexer(vector<string> &arr, string line);

void build_parameters_map();

int main() {
    build_parameters_map();
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
        arr.push_back("EOL\n");
    }
}

void build_parameters_map() {
    parameters["airspeed-indicator_indicated-speed-kt"] = 0;
    parameters["altimeter_indicated-altitude-ft"] = 0;
    parameters["altimeter_pressure-alt-ft"] = 0;
    parameters["attitude-indicator_indicated-pitch-deg"] = 0;
    parameters["attitude-indicator_indicated-roll-deg"] = 0;
    parameters["attitude-indicator_internal-pitch-deg"] = 0;
    parameters["attitude-indicator_internal-roll-deg"] = 0;
    parameters["encoder_indicated-altitude-ft"] = 0;
    parameters["encoder_pressure-alt-ft"] = 0;
    parameters["gps_indicated-altitude-ft"] = 0;
    parameters["gps_indicated-ground-speed-kt"] = 0;
    parameters["gps_indicated-vertical-speed"] = 0;
    parameters["indicated-heading-deg"] = 0;
    parameters["magnetic-compass_indicated-heading-deg"] = 0;
    parameters["slip-skid-ball_indicated-slip-skid"] = 0;
    parameters["turn-indicator_indicated-turn-rate"] = 0;
    parameters["vertical-speed-indicator_indicated-speed-fpm"] = 0;
    parameters["flight_aileron"] = 0;
    parameters["flight_elevator"] = 0;
    parameters["flight_rudder"] = 0;
    parameters["flight_flaps"] = 0;
    parameters["engine_throttle"] = 0;
    parameters["engine_rpm"] = 0;
}