//
// Created by milan on 5.4.2017.
//

#ifndef IPK2_HEADER_HPP
#define IPK2_HEADER_HPP

#include <iostream>
#include <stdio.h>
#include <string>
#include <queue>
#include <string.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <climits>
#include <openssl/md5.h>


#define DEBUG_RED(x, y) clog <<"\033[1;31m"<<x<<": \033[0;0m"<<y<<endl
#define DEBUG_GREEN(x, y) clog <<"\033[1;32m"<<x<<": \033[0;0m"<<y<<endl
#define DEBUG_BLUE(x, y) clog <<"\033[1;34m"<<x<<": \033[0;0m"<<y<<endl
#define DEBUG_YELLOW(x, y) clog <<"\033[1;33m"<<x<<": \033[0;0m"<<y<<endl

#define BUFSIZE 1024
#define PORTNUM "55555"
#define LOGIN "xaugus09"

using namespace std;

void printError(const char *e, int code);
void arguments(int argc, char **argv, string *s);
string md5_hash(string s);

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

class ClientException: public CoreException {
public:
    ClientException(const string &e) throw(): CoreException(e) {}
};

#endif //IPK2_HEADER_HPP
