//
// Created by milan on 8.4.2017.
//

#ifndef IPK2_PROTOCOL_HPP
#define IPK2_PROTOCOL_HPP

#include "header.hpp"

class Protocol {
private:
    queue<string> container;
    string message, package;
    bool err;
    string type, num1, op, num2, secret;
    unsigned find_all(string src, string c);
    int str2num(string num1, string num2, int *n1, int *n2);
    bool isInt(string s);
    inline int power(int x, unsigned n);
    double truncate(double num, unsigned n);
public:
    Protocol();
    ~Protocol(){}
    bool check_add(char *buffer);
    bool check_add(string buffer);
    bool check_add(char *buffer, unsigned bytes);
    void clear();
    void parse();
    bool setAnswer(string msg);
    string get();
    string getType();
    string getSecret();
    bool solve();
    bool status();
    bool empty();
    void split();
};


#endif //IPK2_PROTOCOL_HPP
