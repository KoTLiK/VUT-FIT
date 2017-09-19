//

#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <signal.h>
#include <string.h>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define DEBUG_GREEN(x, y) clog <<"\033[1;32m"<<x<<": \033[0;0m"<<y<<endl
#define DEBUG_RED(x, y) clog <<"\033[1;31m"<<x<<": \033[0;0m"<<y<<endl
#define DEBUG_BLUE(x, y) clog <<"\033[1;34m"<<x<<": \033[0;0m"<<y<<endl
#define DEBUG_YELLOW(x, y) clog <<"\033[1;33m"<<x<<": \033[0;0m"<<y<<endl

#define BUFSIZE 1024
#define PORTNUM 55555
#define LISTEN_QUEUE 10

using namespace std;

void printError(const char *e, int code);
void arguments(int argc, char **argv, string *s1, string *s2);
void getContent(string file, vector<string> *vec);
unsigned find_all(string src, string c);
bool find_login(vector<string> *str, string *login, string hash);

class CoreException: public exception {
private:
    const string s;
public:
    CoreException(const string &e) throw(): s(e) {}
    virtual ~CoreException() throw() {}
    virtual const char * what() const throw() {
        return s.c_str();
    }
};

class ServerException: public CoreException {
public:
    ServerException(const string &e) throw(): CoreException(e) {}
};

#endif //HEADER_HPP
