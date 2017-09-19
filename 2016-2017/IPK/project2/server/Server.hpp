//

#ifndef IPK_MATH_SERVER_SERVER_HPP
#define IPK_MATH_SERVER_SERVER_HPP


#include "header.hpp"
#include "Protocol.hpp"

class Server {
private:
    int socket_desc, connection;
    struct sockaddr_in6 server, client;
    socklen_t length;
    char buffer[BUFSIZE+1];
public:
    Server(){}
    ~Server(){}
    void start();
    void stop();
    void openConnection();
    int getConnectionSocket();
    void closeConnection();
    void closeConnection(int socket);
    void sendMessage(string MSG);
    void sendMessage(string MSG, int socket);
    Protocol *receiveMessage();
    Protocol *receiveMessage(int socket);
    struct sockaddr_in6 *getClient();
};


#endif //IPK_MATH_SERVER_SERVER_HPP
