//
// Created by Minko on 29.9.2017.
//

#ifndef ISA_CONTROLLER_H
#define ISA_CONTROLLER_H


#include "../common/common.h"
#include "../client/ClientProtocol.h"
#include "../client/Client.h"

#define MSG_QUEUE msgReadyToSend[channel.first][nick.first]


class Controller : public ClientProtocol {
private:
    Client syslog;
    std::queue<std::string> container;
    std::string package, message, trailing, currentIP;
    std::map<std::string, std::map<std::string, bool>> channelUserList;
    std::map<std::string, std::map<std::string, std::queue<std::string>>> msgReadyToSend;
    std::vector<std::string> params, highlights;
    bool highlightSet;

    std::regex rx_message, cmd_today, cmd_msg;
    enum { RX_NICK = 1, RX_USER, RX_HOST, RX_COMMAND, RX_PARAMS };

    bool handlePrivmsgAndNotice(std::smatch sm);
    bool addAllOnlineUsers();
    bool addOneOnlineUser(std::smatch sm);
    bool userLeftAllChannels(std::smatch sm);
    bool userLeftOneChannel(std::smatch sm);
    bool userChangedName(std::smatch sm);
    bool userHasBeenKicked();
    bool commands(std::string channel, std::string msg);
    inline bool pingPong();
    inline std::string currentTime(const char *format);
    void splitParamsAndTrailing(const std::string &s);
    std::vector<std::string> splitString(const std::string &string, const char *delim);
    bool checkForHighlight(std::string msg);
    bool findHighlight(const std::string &msg, const std::string &key);
    bool prepareQueuedMessages();
    inline std::string getCurrentIP();
public:
    Controller();
    ~Controller() = default;
    bool appendAndCheck(char *buffer, unsigned bytes) override;
    std::string getMessage() override;
    void split() override;
    bool empty() override;
    std::string front() override;
    void pop() override;
    bool processMessage() override;
    void setHighlighting(std::vector<std::string> highlight);
    void setSyslogAndConnect(const std::string &syslogServer);
};


#endif //ISA_CONTROLLER_H
