//

#include "Server.hpp"

void Server::start() {
    if ((socket_desc = socket(PF_INET6, SOCK_STREAM, 0)) < 0)
        throw ServerException("The socket cannot be created.");

    bzero(&server, sizeof(server));
    server.sin6_family = AF_INET6;
    server.sin6_addr = in6addr_any;
    server.sin6_port = htons(PORTNUM);

    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
        throw ServerException("Cannot bind...not enough mana.");
    if (listen(socket_desc, LISTEN_QUEUE) < 0)
        throw ServerException("I can't hear yooou...");

    length = sizeof(client);
}

void Server::stop() {
    close(socket_desc);
}

void Server::openConnection() {
    if ((connection = accept(socket_desc, (struct sockaddr*)&client, &length)) < 0)
        throw ServerException("Challenge NOT accepted...nor the connection...");
}

int Server::getConnectionSocket() {
    return connection;
}

void Server::closeConnection() {
    close(connection);
}

void Server::closeConnection(int socket) {
    close(socket);
}

void Server::sendMessage(string MSG) {
    if (send(socket_desc, MSG.c_str(), MSG.size(), 0) < 0)
        throw ServerException("Cannot send message to server.");
}

Protocol *Server::receiveMessage() {
    Protocol *MSG = new Protocol;
    for (int i = 0;; i ^= i) {
        bzero(buffer, BUFSIZE+1);
        if ((i = (int)recv(socket_desc, buffer, BUFSIZE, 0)) == 0) break;
        else if (i < 0) throw ServerException("Ooops. Receive goes wrong.");
        else if (MSG->check_add(buffer, (unsigned)i)) break;
    }
    return MSG;
}

sockaddr_in6 *Server::getClient() {
    return &client;
}

void Server::sendMessage(string MSG, int socket) {
    if (send(socket, MSG.c_str(), MSG.size(), 0) < 0)
        throw ServerException("Cannot send message to server.");
}

Protocol *Server::receiveMessage(int socket) {
    Protocol *MSG = new Protocol;
    for (int i = 0;; i ^= i) {
        bzero(buffer, BUFSIZE+1);
        if ((i = (int)recv(socket, buffer, BUFSIZE, 0)) == 0) break;
        else if (i < 0) throw ServerException("Ooops. Receive goes wrong.");
        else if (MSG->check_add(buffer, (unsigned)i)) break;
    }
    return MSG;
}
