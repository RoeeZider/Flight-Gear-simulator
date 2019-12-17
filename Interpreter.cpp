//
// Created by roee on 17/12/2019.
//

#include "Interpreter.h"
using namespace std;

#include <string>
#include <iostream>

#include "Expression.h"
#include <stack>
#include <queue>
#include <regex>
#include <vector>
#include <map>


BinaryOperator::BinaryOperator(Expression *left1, Expression *right1) {
    this->left = left1;
    this->right = right1;
}

BinaryOperator::~BinaryOperator() {
    delete (left);
    delete (right);
}

/*
double BinaryOperator::calculate() {
    //problem
    return this->left->calculate(), this->right->calculate();
}
*/

Plus::Plus(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Plus::calculate() {
    return (left->calculate() + right->calculate());
}

Minus::Minus(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Minus::calculate() {
    return (left->calculate() - right->calculate());
}

Div::Div(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Div::calculate() {
    if (right->calculate() == 0) {
        throw "Bad input";
    }

    return (left->calculate() / right->calculate());

}


Mul::Mul(Expression *left1, Expression *right1) : BinaryOperator(left1, right1) {}

double Mul::calculate() {
    return (left->calculate() * right->calculate());
}


UnaryOperator::UnaryOperator(Expression *exp1) {
    this->exp = exp1;

}

UnaryOperator::~UnaryOperator() {
    delete (exp);
}

/*
double UnaryOperator::calculate() {
    return exp->calculate();
} */

double UPlus::calculate() {
    return exp->calculate();
}

UPlus::UPlus(Expression *exp1) : UnaryOperator(exp1) {
}

UMinus::UMinus(Expression *exp1) : UnaryOperator(exp1) {
}

double UMinus::calculate() {
    return (-1) * exp->calculate();;
}

//UMinus::UMinus(double num) : UnaryOperator(new Value(num)) {
//}


Value::Value(const double num) : value(num) {}

double Value::calculate() {
    return value;
}

////why const double


double Variable::calculate() {
    return value;
}

Variable::Variable(string name1, double num1) : name(name1), value(num1) {}

Variable &Variable::operator++() {
    this->value += 1;
    return *this;
}

Variable &Variable::operator++(int) {
    this->value += 1;
    return *this;
}

Variable &Variable::operator--() {
    this->value -= 1;
    return *this;
}

Variable &Variable::operator--(int) {
    this->value -= 1;
    return *this;
}

Variable &Variable::operator+=(double num) {
    this->value += num;
    return *this;
}

Variable &Variable::operator-=(double num) {
    this->value -= num;
    return *this;
}


Expression *Interpreter::interpret(string str) {
    //arrange the string
    string str2 = changeVar(str);
    list<string> strings = splitTheString(str2);
    queue<string> postfixString = shuntingYard(strings);
    //calculate
    stack<Expression *> stackToEvaluate;

    Expression *num1;
    Expression *num2;

    while (postfixString.size() != 0) {
        if (!isOperator(postfixString.front())) {
            stackToEvaluate.push(new Value(stod(postfixString.front())));
            postfixString.pop();
        } else {
            num1 = stackToEvaluate.top();
            stackToEvaluate.pop();


            if (postfixString.front() == "+") {
                num2 = stackToEvaluate.top();
                stackToEvaluate.pop();
                stackToEvaluate.push(new Plus(num2, num1));
                postfixString.pop();

            } else if (postfixString.front() == "-") {
                num2 = stackToEvaluate.top();
                stackToEvaluate.pop();
                stackToEvaluate.push(new Minus(num2, num1));
                postfixString.pop();

            } else if (postfixString.front() == "*") {
                num2 = stackToEvaluate.top();
                stackToEvaluate.pop();
                stackToEvaluate.push(new Mul(num2, num1));
                postfixString.pop();

            } else if (postfixString.front() == "/") {
                num2 = stackToEvaluate.top();
                stackToEvaluate.pop();
                stackToEvaluate.push(new Div(num2, num1));
                postfixString.pop();

            } else if (postfixString.front() == "$") {
                stackToEvaluate.push(new UMinus(num1));
                postfixString.pop();
            } else if (postfixString.front() == "@") {
                stackToEvaluate.push(new UPlus(num1));
                postfixString.pop();
            }
        }
    }

    return stackToEvaluate.top();

}


list<string> Interpreter::splitTheString(string str) {

    int numLeftBracket = 0;
    int numRightBracket = 0;
    string var = "";
    list<string> strings;
    list<string> vars;
    string newStr = "";
    string oper = "";
    bool flag = false;
    bool flag2 = false;

    //for invalid
    for (unsigned int i = 1; i < str.length(); i++) {
        if (isalpha(str[i])) {
            if (str[i - 1] == '$' || str[i - 1] == '@') {
                throw "Bad input";
            }
        }
    }

    for (unsigned int i = 0; i < str.length(); i++) {
        char ch = str[i];
        if (isNumber(ch)) {
            newStr = newStr + ch;
        } else if (ch == '.') {
            newStr = newStr + ch;
        } else if (isOperator(ch)) {
            //counting the bracket
            if (ch == '(') {
                numLeftBracket++;
            }
            if (ch == ')') {
                numRightBracket++;
            }

            if (i == str.length() - 1 && checkIfOperator(ch)) {
                throw ("Bad input");
            }
            if (checkIfOperator(str[i]) && checkIfOperator(str[i + 1])) {
                throw ("Bad input");
            }
            if (newStr.length() != 0) {
                strings.push_back(newStr);
                newStr = "";
            }
            //convert from char to string
            oper = string(1, ch);
            strings.push_back(oper);

        } else if (isalpha(ch)) {
            flag2 = false;
            flag = false;
            var = "";
            for (unsigned int j = i; j < str.length(); j++) {
                if (!isOperator(str[j])) {
                    var = var + str[j];
                } else {
                    vars.push_back(var);
                    for (std::map<string, double>::iterator it = this->mapVariables.begin();
                         it != this->mapVariables.end(); ++it) {
                        if (it->first == var) {

                            str.replace(i, var.length(), "");
                            std::string value = std::to_string(it->second);
                            strings.push_back(value);
                            //for break
                            flag2 = true;
                            //for found var
                            flag = true;
                            break;

                        }
                    }
                }
                //we finish to check the var
                if (flag2) {
                    i -= var.length();
                    break;
                }
            }
        }


    }

    //if we forgot a var in the end of the string
    if (var.length() != 0 && !flag) {
        for (std::map<string, double>::iterator it = this->mapVariables.begin();
             it != this->mapVariables.end(); ++it) {

            if (it->first == var) {

                std::string value = std::to_string(it->second);
                strings.push_back(value);
            }
        }
        vars.push_back(var);
    }

    if (newStr.length() != 0) {
        strings.push_back(newStr);
    }
    // if flag-or if isnt valid(function) throw an exception
    if ((numLeftBracket != numRightBracket)) {
        throw "Bad input";
    }
    //for uninitialized var
    bool check;
    for (string var1 : vars) {
        check = false;
        for (std::map<string, double>::iterator it = this->mapVariables.begin();
             it != this->mapVariables.end(); ++it) {
            if (it->first == var1) {
                check = true;
            }
        }
        if (!check) {
            throw "Bad input";
        }
    }


    return strings;
}


void Interpreter::setVariables(string str) {

    string var = "";
    string value = "";
    list<string> variables;
    //valid regex
    regex validVar("[a-zA-Z]+[_a-zA-Z0-9]*");
    regex validVal("-?[0-9].?[0-9]*");

    bool flag = false;
    string exp = "";
    string left = "";
    for (char ch :str) {
        if (ch == ';') {
            for (char c : exp) {
                if (c == '=') {
                    left = var;
                    var = "";
                } else {
                    var = var + c;
                }
            }
            //insert to map the var
            if (regex_match(left, validVar) && regex_match(var, validVal)) {
                for (std::map<string, double>::iterator it = this->mapVariables.begin();
                     it != this->mapVariables.end(); ++it) {
                    flag = false;
                    if (it->first == left) {
                        it->second = stod(var);
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    this->mapVariables.insert({left, stod(var)});
                }
            } else {
                throw "Bad input";
            }
            exp = "";
            var = "";

        } else {
            exp = exp + ch;
        }
    }
    if (exp.size() != 0) {
        for (char c : exp) {
            if (c == '=') {
                left = var;
                var = "";
            } else {
                var = var + c;
            }
        }
        if (regex_match(left, validVar) && regex_match(var, validVal)) {
            for (std::map<string, double>::iterator it = this->mapVariables.begin();
                 it != this->mapVariables.end(); ++it) {
                flag = false;
                if (it->first == left) {
                    it->second = stod(var);
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                this->mapVariables.insert({left, stod(var)});
            }
        } else {
            throw "Bad input";
        }
    }


}

bool Interpreter::isNumber(char ch) {
    return (ch >= '0') && (ch <= '9');
}

bool Interpreter::isOperator(char ch) {
    return (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == ')') || (ch == '(') || (ch == '$') ||
           (ch == '@');
}

bool Interpreter::isOperator(string str) {
    return (str == "+") || (str == "-") || (str == "*") || (str == "/") || (str == "$") || (str == "@");
}

int Interpreter::precedence(string str) {
    if ((str == "+") || (str == "-")) {
        return 1;
    } else if ((str == "*") || (str == "/")) {
        return 2;
        //for unary
    } else if ((str == "@") || (str == "$")) {
        return 3;
    } else {
        //(str == "(")
        return 0;
    }
}

//shunting yard algorithm
queue<string> Interpreter::shuntingYard(list<string> strings) {
    stack<string> myStack;
    queue<string> myQueue;
    string ope;
    for (string s : strings) {
        if (!isOperator(s) && (s != "(") && (s != ")")) {
            myQueue.push(s);
        }
        if (isOperator(s)) {
            while (!myStack.empty() && precedence(s) <= precedence(myStack.top())) {
                myQueue.push(myStack.top());
                myStack.pop();
            }
            myStack.push(s);
        }
        if (s == "(") {
            myStack.push(s);
        }
        if (s == ")") {
            while (myStack.top() != "(") {
                myQueue.push(myStack.top());
                myStack.pop();
            }
            myStack.pop();
        }
    }
    while (!myStack.empty()) {
        myQueue.push(myStack.top());
        myStack.pop();
    }
    return myQueue;
}

string Interpreter::changeVar(string str) {

    //for unary exp
    for (unsigned int i = 0; i < str.length(); i++) {
        if (str[i] == '-') {
            if (i == 0) {
                str[i] = '$';
            } else {
                if (str[i - 1] == '(') {
                    str[i] = '$';
                }
            }

        }
        if (str[i] == '+') {
            if (i == 0) {
                str[i] = '@';
            } else {
                if (str[i - 1] == '(') {
                    str[i] = '@';
                }
            }

        }

    }

    return str;
}

bool Interpreter::checkIfOperator(char ch) {
    return (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '$') ||
           (ch == '@');
}
