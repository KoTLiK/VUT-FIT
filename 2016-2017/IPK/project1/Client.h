//
// Created by milan on 7.3.2017.
//

#ifndef IPK_CLIENT_H
#define IPK_CLIENT_H

#include "CoreException.h"
#include "Message.h"

class Client {
private:
    int socket_desc, send_bytes;
    struct sockaddr_in server_address;
    char buffer[BUFSIZE+1];
public:
    Client();
    ~Client();
    void setServer(string hostname, unsigned short port);
    void makeConnection();
    void sendMessage(string MSG);
    Message *receiveMessage();
};


#endif //IPK_CLIENT_H
