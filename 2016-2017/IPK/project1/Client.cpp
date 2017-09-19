//
// Created by milan on 7.3.2017.
//

#include "Client.h"

Client::Client() {}

Client::~Client() {
    close(socket_desc);
}

void Client::setServer(string hostname, unsigned short port) {
    struct hostent *server;
    if ((server = gethostbyname(hostname.c_str())) == NULL)
        throw ClientException("Houston, we've got a problem...gethostbyname problem.");

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy(server->h_addr, (char *)&server_address.sin_addr.s_addr, (size_t)server->h_length);
    server_address.sin_port = htons(port);
}

void Client::makeConnection() {
    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw ClientException("The socket cannot be created. Try later.");
    if (connect(socket_desc, (const struct sockaddr *) &server_address, sizeof(server_address)))
        throw ClientException("Cannot connect to the server. Go make some coffee and try again later.");
}

void Client::sendMessage(string MSG) {
    if ((send_bytes = (int)send(socket_desc, MSG.c_str(), MSG.size(), 0)) < 0)
        throw  ClientException("Cannot send message to server. Call emergency services for help.");
    shutdown(socket_desc, SHUT_WR);
}

Message *Client::receiveMessage() {
    Message *MSG = new Response_Message;
    for (int i = 0;; i ^= i) {
        bzero(buffer, BUFSIZE+1);
        if ((i = (int)recv(socket_desc, buffer, BUFSIZE, 0)) == 0) break;
        else if (i < 0) throw RecvMsgException("Ooops. Something goes wrong.");
        else MSG->buffUp(buffer, (unsigned)i);
    }
    return MSG;
}
