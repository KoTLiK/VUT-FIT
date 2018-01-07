//
// Created by Minko on 29.9.2017.
//

#include "Controller.h"


Controller::Controller() {
    syslog = Client(SOCK_DGRAM);
    highlightSet = false;
    package.clear();
    message.clear();
    /**                            < ------------ prefix ------------ >         < --- command --- >  <params>
     *                                    1.          2.           3.                   4.           5.       */
    rx_message = std::regex(R"(^(?:\:(?:(\S+?)(?:\!(\S+?))?(?:\@(\S+?))?)[ ]+?)?([a-zA-Z]+|[0-9]{3})(.+)\r\n)");
    cmd_today = std::regex(R"(\?today)");
    cmd_msg = std::regex(R"(\?msg ([^ ]+):(.+))");
}

bool Controller::appendAndCheck(char * const buffer,const  unsigned bytes) {
    package += std::string(buffer, bytes);
    return package.find(CRLF) != std::string::npos;
}

std::string Controller::getMessage() {
    return message;
}

void Controller::split() {
    unsigned long nl;
    std::string temp;
    while ((nl = package.find(CRLF, 0)) != std::string::npos) {
        temp = package.substr(0, nl+2);
        package = package.substr(nl+2);
        container.push(temp);
    }
}

bool Controller::empty() {
    return container.empty();
}

std::string Controller::front() {
    return (message = container.front());
}

void Controller::pop() {
    container.pop();
}

bool Controller::processMessage() {
    std::smatch sm;
    if (std::regex_match(message, sm, rx_message)) {
        std::string cmd = sm[RX_COMMAND];
        splitParamsAndTrailing(sm[RX_PARAMS]);

        if (cmd == "PING") {
            return pingPong();
        } else if (cmd == "PRIVMSG") {
            return handlePrivmsgAndNotice(sm);
        } else if (cmd == "NOTICE") {
            return handlePrivmsgAndNotice(sm);
        } else if (cmd == "353") {
            return addAllOnlineUsers();
        } else if (cmd == "JOIN") {
            return addOneOnlineUser(sm);
        } else if (cmd == "QUIT") {
            return userLeftAllChannels(sm);
        } else if (cmd == "PART") {
            return userLeftOneChannel(sm);
        } else if (cmd == "NICK") {
            return userChangedName(sm);
        } else if (cmd == "KICK") {
            return userHasBeenKicked();
        } else if (cmd[0] == '4' || cmd[0] == '5') {
            std::string errMsg = sm[RX_PARAMS];
            throw BotException(cmd + errMsg);
        } else if (cmd == "ERROR") {
            throw BotException(sm[RX_PARAMS]);
        }
    }
    return false;
}

inline bool Controller::pingPong() {
    message.replace(0, 4, "PONG");
    return true;
}

bool Controller::commands(const std::string channel, const std::string msg) {
    std::smatch sm;
    if (channel[0] != '#' && channel[0] != '&')
        return false;

    if (std::regex_match(msg, sm, cmd_today)) {
        message = "PRIVMSG "+channel+" :"+ currentTime("%d.%m.%Y") +CRLF;
        return true;
    } else if (std::regex_match(msg, sm, cmd_msg)) {
        std::string nick = sm[1];
        std::string text = sm[2];
        std::string msg_to_send = "PRIVMSG "+channel+" :"+nick+":"+text+CRLF;

        if (nick == XLOGIN) return false;

        msgReadyToSend[channel][nick].push(msg_to_send);

        return prepareQueuedMessages();
    } else return false;
}

inline std::string Controller::currentTime(const char *format) {
    char buffer[20];
    time_t t = time(nullptr);
    bzero(&buffer, sizeof(buffer));
    strftime(buffer, sizeof(buffer), format, localtime(&t));
    return std::string(buffer);
}

void Controller::splitParamsAndTrailing(const std::string &s) {
    int stage, i;
    trailing.clear();
    params.clear();
    std::string middle = "";
    const char *buff = s.c_str();
    for (stage = i = 0; buff[i] != '\0'; ++i) {
        switch (stage) {
            case 0: if (buff[i] == ' ') break;
                else if (buff[i] == ':') stage = 1;
                else {
                    middle += buff[i];
                    stage = 2;
                }
                break;
            case 1: trailing += buff[i]; break;
            case 2: if (buff[i] == ' ') {
                    params.emplace_back(middle);
                    middle.clear();
                    stage = 0;
                } else middle += buff[i];
                break;
        }
    }
    if (!middle.empty())
        params.emplace_back(middle);
}

std::vector<std::string> Controller::splitString(const std::string &string, const char *delim) {
    char *token = strtok(const_cast<char *>(string.c_str()), delim);
    std::vector<std::string> list;
    while (token != nullptr) {
        list.emplace_back(std::string(token));
        token = strtok(nullptr, delim);
    }
    return list;
}

bool Controller::prepareQueuedMessages() {
    std::string package = "";
    for (auto &channel : channelUserList) {
        for (auto &nick : channel.second) {
            if (nick.second && !MSG_QUEUE.empty()) {
                while (!MSG_QUEUE.empty()) {
                    package += MSG_QUEUE.front();
                    MSG_QUEUE.pop();
                }
            }
        }
    }

    if (!package.empty()) {
        message = package;
        return true;
    } else return false;
}

void Controller::setHighlighting(std::vector<std::string> highlight) {
    highlights = highlight;
    highlightSet = true;
}

bool Controller::findHighlight(const std::string &msg, const std::string &key) {
    const unsigned long i = msg.find(key);
    const unsigned long key_len = key.length();
    if (i == std::string::npos) {
        return false;
    } else if (i == 0 && (msg[key_len] == ' ' || msg.length() == key_len)) {
        return true;
    } else return ((msg[i - 1] == ' ') && ((msg[i + key_len] == ' ') || (msg.length() == i + key_len)));
}

bool Controller::checkForHighlight(std::string msg) {
    for (auto &item : highlights) {
        if (findHighlight(msg, item))
            return true;
    }
    return false;
}

bool Controller::handlePrivmsgAndNotice(std::smatch sm) {
    if (highlightSet && checkForHighlight(trailing)) {
        std::string nick = sm[RX_NICK];
        std::string header = currentTime("%b %e %H:%M:%S ") + currentIP;
        std::string msg = " isabot: "+nick+":" + trailing;
        syslog.sendMessage((PRI + header + msg).substr(0, 1024));
    }
    return commands(params.back(), trailing);
}

bool Controller::addAllOnlineUsers() {
    for (auto &nick : splitString(trailing, " ")) {
        if (nick[0] == '@' || nick[0] == '+')
            nick = nick.substr(1);
        channelUserList[params.back()][nick] = true;
    }
    return false;
}

bool Controller::addOneOnlineUser(std::smatch sm) {
    std::string nick = sm[RX_NICK];
    channelUserList[params.back()][nick] = true;
    return prepareQueuedMessages();
}

bool Controller::userLeftAllChannels(std::smatch sm) {
    std::string nick = sm[RX_NICK];
    for (auto &item : channelUserList)
        item.second[nick] = false;
    return false;
}

bool Controller::userLeftOneChannel(std::smatch sm) {
    std::string nick = sm[RX_NICK];
    channelUserList[params.back()][nick] = false;
    return false;
}

bool Controller::userChangedName(std::smatch sm) {
    std::string nick = sm[RX_NICK];
    for (auto &item : channelUserList) {
        item.second[nick] = false;
        item.second[trailing] = true;
    }
    return prepareQueuedMessages();
}

bool Controller::userHasBeenKicked() {
    channelUserList[params.front()][params.back()] = false;
    return false;
}

void Controller::setSyslogAndConnect(const std::string &syslogServer) {
    syslog.setConnection(syslogServer, SYSLOGPORT);
    syslog.connectToServer();
    currentIP = getCurrentIP();
}

inline std::string Controller::getCurrentIP() {
    union {
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
        struct sockaddr sa;
    } socket{};

    union {
        char in[INET_ADDRSTRLEN];
        char in6[INET6_ADDRSTRLEN];
    } addr{};

    socklen_t length = sizeof(socket);

    if (getsockname(syslog.getSocketDescriptor(), (struct sockaddr*)&socket, &length) != 0)
        throw BotException("Cannot resolve my own address.");

    switch (socket.sa.sa_family) {
        case AF_INET:
            if (inet_ntop(socket.sin.sin_family, &(socket.sin.sin_addr), addr.in, sizeof(addr.in)) == nullptr)
                throw BotException("Unable to convert my binary name.");
            return std::string(addr.in);
        case AF_INET6:
            if (inet_ntop(socket.sin6.sin6_family, &(socket.sin6.sin6_addr), addr.in6, sizeof(addr.in6)) == nullptr)
                throw BotException("Unable to convert my binary name.");
            return std::string(addr.in6);
        default:
            throw BotException("Unknown protocol");
    }
}
