//
// Created by Minko on 29.9.2017.
//

#include "Client.h"

Client::Client() {
    datagram = SOCK_STREAM;
}

Client::Client(int datagram_socket) {
    datagram = datagram_socket;
}

Client::~Client() {
    close(socket_desc);
}

void Client::setConnection(std::string server_ip, std::string port) {
    bzero(&hints, sizeof(hints));
    hints.ai_flags = AI_NUMERICSERV; /* Only numeric service (port) */
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = datagram;    /* Datagram socket */
    hints.ai_protocol = 0;           /* Any protocol */

    if ((err = getaddrinfo(server_ip.c_str(), port.c_str(), &hints, &result)) != 0)
        throw BotException(gai_strerror(err));
}

void Client::connectToServer() {
    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        if ((socket_desc = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0)
            continue; // Try next one
        if (connect(socket_desc, rp->ai_addr, rp->ai_addrlen) == 0)
            break; // Success
        close(socket_desc);
    }

    if (rp == nullptr) {
        if (datagram == SOCK_DGRAM)
            throw BotException("UDP Service is unavailable.");
        else throw BotException("Cannot connect to the server.");
    }

    freeaddrinfo(result); // Free memory
}

void Client::sendMessage(std::string MSG) {
    if (send(socket_desc, MSG.c_str(), MSG.size(), 0) < 0)
        throw BotException("Cannot send message to server.");
}

void Client::receiveMessage(ClientProtocol *p) {
    if (p->empty()) {
        for (int i = 0;; i ^= i) {
            bzero(buffer, BUFSIZE + 1);
            if ((i = (int) recv(socket_desc, buffer, BUFSIZE, 0)) == 0) break;
            else if (i < 0) throw BotException("Ooops. Receive goes wrong.");
            else if (p->appendAndCheck(buffer, (unsigned) i)) break;
        }
    }
    p->split();
}

int Client::getSocketDescriptor() {
    return socket_desc;
}
