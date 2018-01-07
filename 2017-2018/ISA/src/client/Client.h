//
// Created by Minko on 29.9.2017.
//

#ifndef ISA_CLIENT_H
#define ISA_CLIENT_H

#include "../common/common.h"
#include "ClientProtocol.h"

class Client {
private:
    int socket_desc, datagram, err;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char buffer[BUFSIZE+1];
public:
    Client();
    explicit Client(int datagram_socket);
    ~Client();
    void setConnection(std::string server_ip, std::string port);
    void connectToServer();
    void sendMessage(std::string MSG);
    void receiveMessage(ClientProtocol *p);
    int getSocketDescriptor();
};


#endif //ISA_CLIENT_H
