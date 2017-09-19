//
// Created by milan on 5.4.2017.
//

#include "Client.hpp"

Client::Client() {}

Client::~Client() {
    close(socket_desc);
}

void Client::setConnection(string server_ip) {
    bzero(&hints, sizeof(hints));
    hints.ai_flags = AI_NUMERICSERV; /* Only numeric service (port) */
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_protocol = 0;           /* Any protocol */

    if ((err = getaddrinfo(server_ip.c_str(), PORTNUM, &hints, &result)) != 0)
        throw ClientException(gai_strerror(err));
}

void Client::connectToServer() {
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if ((socket_desc = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol /* TODO nahradit za 0 ? */)) < 0)
            continue; // Try next one
        if (connect(socket_desc, rp->ai_addr, rp->ai_addrlen) == 0)
            break; // Success
        close(socket_desc);
    }

    if (rp == NULL) throw ClientException("Cannot connect to the server.");

    freeaddrinfo(result); // Free memory
}

void Client::sendMessage(string MSG) {
    if (send(socket_desc, MSG.c_str(), MSG.size(), 0) < 0)
        throw ClientException("Cannot send message to server.");
}

void Client::receiveMessage(Protocol *p) {
    if (p->empty())
        for (int i = 0;; i ^= i) {
            bzero(buffer, BUFSIZE+1);
            if ((i = (int)recv(socket_desc, buffer, BUFSIZE, 0)) == 0) break;
            else if (i < 0) throw ClientException("Ooops. Receive goes wrong.");
            else if (p->check_add(buffer, (unsigned)i)) break;
        }
    p->split();
}
