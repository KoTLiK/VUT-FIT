//
// Created by milan on 8.4.2017.
//

#include "Protocol.hpp"

Protocol::Protocol() {
    message.clear();
    package.clear();
    err = false;
    type.clear();
    num1.clear();
    op.clear();
    num2.clear();
    secret.clear();
}

bool Protocol::check_add(char *buffer) {
    package += string(buffer);
    return package.find('\n') != string::npos;
}

bool Protocol::check_add(string buffer) {
    package += buffer;
    return package.find('\n') != string::npos;
}

bool Protocol::check_add(char *buffer, unsigned bytes) {
    package += string(buffer, bytes);
    return package.find('\n') != string::npos;
}

bool Protocol::setAnswer(string msg) {
    message.clear();
    message = msg;
    return message.find('\n') != string::npos;
}

string Protocol::get() {
    return message;
}

string Protocol::getType() {
    return type;
}

string Protocol::getSecret() {
    return secret;
}

void Protocol::clear() {
    message.clear();
    err = false;
    type.clear();
    num1.clear();
    op.clear();
    num2.clear();
    secret.clear();
}

void Protocol::parse() {
    message = container.front();
    container.pop();

    unsigned long i;
    string temp;
    if (message.find("BYE ") == 0) {
        /*if (this->find_all(message, " ") != 1) {
            err = true;
            return;
        }*/
        i = message.find(" ");
        type = message.substr(0, i);
        secret = message.substr(i+1);
        secret[secret.length()-1] = '\0';
        secret = string(secret.c_str());
    } else if (message.find("SOLVE ") == 0) {
        if (find_all(message, " ") != 3) {
            err = true;
            return;
        }
        i = message.find(" ");
        type = message.substr(0, i); // SOLVE
        temp = message.substr(i+1);

        i = temp.find(" ");
        num1 = temp.substr(0, i); // NUM1
        temp = temp.substr(i+1);

        i = temp.find(" ");
        op = temp.substr(0, i); // OPERATOR
        temp = temp.substr(i+1);

        i = temp.find("\n");
        num2 = temp.substr(0, i); // NUM2
    } else err = true;
}

bool Protocol::solve() {
    if (op.size() != 1) return false;
    char str[100];
    bzero(str, 100);
    int n1, n2;
    switch (op[0]) {
        case '+':
            if (str2num(num1, num2, &n1, &n2)) return false;
            else sprintf(str, "%.2lf", (double)(n1 + n2));
            break;
        case '-':
            if (str2num(num1, num2, &n1, &n2)) return false;
            else sprintf(str, "%.2lf", (double)(n1 - n2));
            break;
        case '*':
            if (str2num(num1, num2, &n1, &n2)) return false;
            else sprintf(str, "%.2lf", (double)(n1 * n2));
            break;
        case '/':
            if (str2num(num1, num2, &n1, &n2)) return false;
            else if (n2 != 0)
                sprintf(str, "%.2lf", truncate((n1 / (double)n2), 2));
            else return false; // Division by ZERO !
            break;
        default:
            return !(err = true);
    }
    
    setAnswer("RESULT "+ string(str) +"\n");
    return true;
}

unsigned Protocol::find_all(string src, string c) {
    unsigned long p = src.find(c, 0);
    unsigned count = 0;

    while (p != string::npos) {
        p = src.find(c, p+c.size());
        count++;
    }

    return count;
}

int Protocol::str2num(string num1, string num2, int *n1, int *n2) {
    char *end_ptr = NULL;

    if (isInt(num1)) {
        long long int num = strtoll(num1.c_str(), &end_ptr, 10);
        if (*end_ptr != '\0') return -1;
        if (num > INT_MAX || num < INT_MIN) return -1;
        *n1 = (int) num;
    } else return 1;

    if (isInt(num2)) {
        long long int num = strtoll(num2.c_str(), &end_ptr, 10);
        if (*end_ptr != '\0') return -1;
        if (num > INT_MAX || num < INT_MIN) return -1;
        *n2 = (int) num;
    } else return 1;

    return 0;
}

bool Protocol::isInt(string s) {
    const char *c = s.c_str();
    enum {OPTIONAL, DIGIT, DIGIT2};
    int state = OPTIONAL;

    unsigned i;
    for (i = 0; c[i] != '\0'; ++i) {
        switch (state) {
            case OPTIONAL:
                if (c[i] == '+' || c[i] == '-') state = DIGIT;
                else if (isdigit(c[i])) state = DIGIT2;
                else return !(err = true);
                break;
            case DIGIT:
                if (isdigit(c[i])) state = DIGIT2;
                else return !(err = true);
                break;
            case DIGIT2:
                if (!isdigit(c[i])) return !(err = true);
                break;
        }
    }

    return i < 11;
}

bool Protocol::status() {
    return err;
}

bool Protocol::empty() {
    return container.empty();
}

void Protocol::split() {
    unsigned long nl;
    string temp;
    while ((nl = package.find("\n", 0)) != string::npos) {
        temp = package.substr(0, nl+1);
        package = package.substr(nl+1);
        container.push(temp);
    }
}

inline int Protocol::power(int x, unsigned n) {
    int i = 1;
    while (n-- > 0) i *= x;
    return i;
}

double Protocol::truncate(double num, unsigned digit) {
    double f = power(10, digit);
    return ((int)(num * f)) / f;
}
