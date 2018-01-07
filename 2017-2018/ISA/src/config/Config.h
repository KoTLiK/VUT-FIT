//
// Created by Minko on 28.9.2017.
//

#ifndef ISA_CONFIG_H
#define ISA_CONFIG_H

#include "../common/common.h"

class Config {
private:
    std::string host, port, syslog_server, raw_channels;
    std::vector<std::string> channels, highlight;
    bool high;
    void split(std::vector<std::string> *list, std::string s);
public:
    Config();
    ~Config() = default;
    Config *setHostAndPort(std::string s);
    Config *setChannels(std::string s);
    Config *setSyslogServer(std::string s);
    Config *setHighlights(std::string s);
    bool check();
    std::string getHost();
    std::string getPort();
    std::vector<std::string> getChannels();
    std::string getRawChannels();
    std::string getSyslogServer();
    std::vector<std::string> getHighlights();
    bool isHighlighting();
};


#endif //ISA_CONFIG_H
