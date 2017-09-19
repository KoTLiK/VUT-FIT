//
// Created by milan on 7.3.2017.
//

#ifndef IPK_COREEXCEPTION_H
#define IPK_COREEXCEPTION_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <string>
#include <time.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>

#define DEBUG_GREEN(x, y) clog <<"\033[0;32m"<<x<<": \033[0;0m"<<y<<endl
#define DEBUG_RED(x, y) clog <<"\033[0;31m"<<x<<": \033[0;0m"<<y<<endl

using namespace std;

void printError(const char *e, int code);

class CoreException: public exception {
private:
    const string s;
public:
    CoreException(const string &e) throw(): s(e) {}
    virtual ~CoreException() throw() {}
    virtual const char *what() const throw() {
        return s.c_str();
    }
};

class ArgsException: public CoreException {
public:
    ArgsException(const string &e) throw(): CoreException(e) {}
};

class ServerException: public CoreException {
public:
    ServerException(const string &e) throw(): CoreException(e) {}
};

class RecvMsgException: public CoreException {
public:
    RecvMsgException(const string &e) throw(): CoreException(e) {}
};

class ClientException: public CoreException {
public:
    ClientException(const string &e) throw(): CoreException(e) {}
};

#endif //IPK_COREEXCEPTION_H
