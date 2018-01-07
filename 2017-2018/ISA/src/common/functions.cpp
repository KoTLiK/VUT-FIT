//
// Created by Minko on 26.9.2017.
//


#include "functions.h"


void printError(const char *e, const int code) {
    std::cerr << "isabot: ERROR> "<< e << std::endl;
    exit(code);
}

Config *arguments(int argc, char **argv) {
    if (argc == 1) return nullptr;

    int c;
    Config *config = new Config();
    std::string syslog, highlight;
    struct option o { "help", no_argument, nullptr, 42 };

    if (argc >= 3) {
        config->setHostAndPort(argv[1]);
        config->setChannels(argv[2]);
    }

    while ((c = getopt_long(argc, argv, "hs:l:", &o, nullptr)) != -1) {
        switch (c) {
            case 'h':
            case 42:
                delete config;
                if (argc != 2) return nullptr;
                else {
                    std::cout << HELP << std::endl;
                    exit(0);
                }
            case 's':
                if (argc > 3) {
                    if (optarg) syslog = optarg;
                    config->setSyslogServer(optarg);
                    break;
                } else {
                    delete config;
                    return nullptr;
                }
            case 'l':
                if (argc > 3) {
                    if (optarg) highlight = optarg;
                    config->setHighlights(optarg);
                    break;
                } else {
                    delete config;
                    return nullptr;
                }
            default:
                delete config;
                return nullptr;
        }
    }

    return config;
}
