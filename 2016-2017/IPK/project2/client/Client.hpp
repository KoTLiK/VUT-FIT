//
// Created by milan on 5.4.2017.
//

#ifndef IPK2_CLIENT_HPP
#define IPK2_CLIENT_HPP

#include "header.hpp"
#include "Protocol.hpp"

class Client {
private:
    int socket_desc, err;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char buffer[BUFSIZE+1];
public:
    Client();
    ~Client();
    void setConnection(string server_ip);
    void connectToServer();
    void sendMessage(string MSG);
    void receiveMessage(Protocol *p);
};


#endif //IPK2_CLIENT_HPP
