//
// Created by milan on 7.3.2017.
//

#include "Server.h"

Server::Server() {}

Server::~Server() {
    close(socket_desc);
}

void Server::start() {
    if ((socket_desc = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        throw ServerException("The socket cannot be created. Try later.");

    memset(&server, 0, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
        throw ServerException("Cannot bind...not enough mana.");
    if (listen(socket_desc, 1) < 0)
        throw ServerException("I can't hear yooou...");

    length = sizeof(client);
}

void Server::openConnection() {
    if ((connection = accept(socket_desc, (struct sockaddr*)&client, &length)) < 0)
        throw ServerException("Challenge NOT accepted...nor the connection...");
}

void Server::closeConnection() {
    close(connection);
}

void Server::setPort(int num) {
    port = (unsigned short)num;
}

Message *Server::receiveMessage() {
    Message *MSG = new Response_Message;
    for (int i = 0;; i ^= i) {
        bzero(buffer, BUFSIZE+1);
        if ((i = (int)recv(connection, buffer, BUFSIZE, 0)) == 0) break;
        else if (i < 0) throw RecvMsgException("Ooops. Something goes wrong.");
        else MSG->buffUp(buffer, (unsigned)i);
    }
    return MSG;
}

void Server::sendMessage(string MSG) {
    if ((send_bytes = (int)send(connection, MSG.c_str(), MSG.size(), 0)) < 0)
        throw ClientException("Cannot send message to server. Call emergency services for help.");
}
