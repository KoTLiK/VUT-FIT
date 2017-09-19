//
// Created by milan on 7.3.2017.
//

#ifndef IPK_SERVER_H
#define IPK_SERVER_H

#include "CoreException.h"
#include "Message.h"

class Server {
private:
    unsigned short port;
    int socket_desc, connection, send_bytes;
    struct sockaddr_in server, client;
    socklen_t length;
    char buffer[BUFSIZE+1];
public:
    Server();
    ~Server();
    void start();
    void openConnection();
    void closeConnection();
    void setPort(int num);
    Message *receiveMessage();
    void sendMessage(string MSG);
};


#endif //IPK_SERVER_H
