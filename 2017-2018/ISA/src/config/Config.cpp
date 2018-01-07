//
// Created by Minko on 28.9.2017.
//

#include "Config.h"

Config::Config() {
    port = PORTNUM;
    syslog_server = SYSLOG_DEFAULT;
    high = false;
}

Config *Config::setHostAndPort(std::string s) {
    host = s.substr(0, s.find(':'));
    if (s.find(':') != std::string::npos) {
        std::string tmp_port = s.substr(s.find(':')+1);
        port = tmp_port;
    }
    return this;
}

Config *Config::setChannels(std::string s) {
    raw_channels = s;
    if (s.length() != 0)
        split(&channels, s);
    return this;
}

Config *Config::setSyslogServer(std::string s) {
    syslog_server = s;
    return this;
}

Config *Config::setHighlights(std::string s) {
    if (s.length() != 0)
        split(&highlight, s);
    high = true;
    return this;
}

bool Config::check() {
    std::smatch sm;
    std::regex e (R"([#&][^\s]+)");
    for (auto &item : channels) {
        if (!std::regex_match(item, sm, e))
            return false;
    }

    for (auto &item : highlight)
        if (item.empty()) return false;

    if ((!high || highlight.empty()) && (high || !highlight.empty())) return false;

    return !(port.empty() || channels.empty() || syslog_server.empty());
}

std::string Config::getHost() {
    return host;
}

std::string Config::getPort() {
    return port;
}

std::vector<std::string> Config::getChannels() {
    return channels;
}

std::string Config::getRawChannels() {
    return raw_channels;
}

std::string Config::getSyslogServer() {
    return syslog_server;
}

std::vector<std::string> Config::getHighlights() {
    return highlight;
}

bool Config::isHighlighting() {
    return high;
}

void Config::split(std::vector<std::string> *list, std::string s) {
    std::string temp = s;
    int i;
    while ((i = temp.find(',')) != (int) std::string::npos) {
        list->emplace_back(temp.substr(0, i));
        temp = temp.substr(i+1);
    }
    list->emplace_back(temp.substr(0, i));
}
