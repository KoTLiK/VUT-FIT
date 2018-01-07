//
// Created by Minko on 26.9.2017.
//

#ifndef ISA_COMMON_H
#define ISA_COMMON_H

#include <string>
#include <iostream>
#include <queue>
#include <tuple>
#include <list>
#include <string.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <regex>
#include <time.h>
#include <map>

#define CRLF "\r\n"
#define BUFSIZE 1024
#define PORTNUM "6667"

#define SYSLOGPORT "514"
#define SYSLOG_DEFAULT "127.0.0.1"
#define PRI "<134>"

#define XLOGIN "xaugus09"
#define NICK "NICK " XLOGIN
#define USER "USER " XLOGIN " " XLOGIN " " XLOGIN " :" XLOGIN


class BotException: public std::exception {
private:
    const std::string s;
public:
    BotException(const std::string &e) throw(): s(e) {}
    virtual ~BotException() throw() {}
    virtual const char * what() const throw() {
        return s.c_str();
    }
};

#endif //ISA_COMMON_H
