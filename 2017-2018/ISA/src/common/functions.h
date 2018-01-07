//
// Created by Minko on 28.9.2017.
//

#ifndef ISA_FUNCTIONS_H
#define ISA_FUNCTIONS_H

#include "common.h"
#include "../config/Config.h"

static const std::string HELP = "IRC Client:\n"
        "  Usage: ./isabot HOST[:PORT] CHANNELS [-s SYSLOG_SERVER] [-l HIGHLIGHT] [-h|--help]\n"
        "    HOST - server name\n"
        "    PORT - default is 6667\n"
        "    CHANNELS - list of channel name (starting with '#' or '&') separated with comma\n"
        "    -s SYSLOG_SERVER - IP address of SYSLOG server\n"
        "    -l HIGHLIGHT - list of keywords separated with comma\n"
        "    -h|--help - prints this help\n"
        "  Made by xaugus09 with <3";
static const std::string USAGE = "wrong usage!\n"
        "  Usage: ./isabot HOST[:PORT] CHANNELS [-s SYSLOG_SERVER] [-l HIGHLIGHT] [-h|--help]\n"
        "  more with -h|--help";

void printError(const char *e, const int code);
Config *arguments(int argc, char **argv);

#endif //ISA_FUNCTIONS_H
