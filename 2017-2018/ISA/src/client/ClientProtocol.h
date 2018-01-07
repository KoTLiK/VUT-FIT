//
// Created by Minko on 6.10.2017.
//

#ifndef ISA_CLIENT_PROTOCOL_H
#define ISA_CLIENT_PROTOCOL_H

class ClientProtocol {
public:
    virtual bool appendAndCheck(char *buffer, unsigned bytes) = 0;
    virtual std::string getMessage() = 0;
    virtual void split() = 0;
    virtual bool empty() = 0;
    virtual std::string front() = 0;
    virtual void pop() = 0;
    virtual bool processMessage() = 0;
};

#endif //ISA_CLIENT_PROTOCOL_H
