//
// Created by milan on 7.3.2017.
//

#ifndef IPK_MESSAGE_H
#define IPK_MESSAGE_H

#include "CoreException.h"
#include "ARGS.h"

/// 9001 - prime, don't ask, 8kiB was very obvious . Content size of multiples of BUFSIZE will cause to stuck at recv()
#define BUFSIZE 9001

enum {
    OCTET_STREAM = 0,
    TEXT,
    IDENTITY,
    _200,
    _400,
    _403,
    _404,
    _409,
    _500,
    NDIR,
    NFILE,
    FILENF,
    DIRNF,
    DIRNE,
    EXIST,
    UACC,
    UNKNOWN
};

const string INFO_TYP[] = {
    "application/octet-stream",
    "text/plain",
    "identity",
    "200 OK",
    "400 Bad Request",
    "403 Forbidden",
    "404 Not Found",
    "409 Conflict",
    "500 Internal Server Error",
    "Not a directory.",
    "Not a file.",
    "File not found.",
    "Directory not found.",
    "Directory not empty.",
    "Already exists.",
    "User account not found.",
    "Unknown error."
};

class Message {
protected:
    string message;
public:
    Message();
    virtual ~Message(){}
    virtual void buffUp(char *buffer);
    virtual void buffUp(string buffer);
    virtual void buffUp(char *buffer, unsigned bytes);
    virtual string blowUp();
    virtual void decode(){}
    virtual void decodeResponse(){}
    virtual string getCMD(){ return nullptr; }
    virtual string getPath(){ return nullptr; }
    virtual string getType(){ return nullptr; }
    virtual string getData(){ return nullptr; }
    virtual unsigned getLength(){ return 0; }
    virtual void addInfo(string h, string a, string encoding, string type){}
    virtual void setContent(time_t timestamp, string content){}
};

class Request_Message: public Message {
private:
    string request_header, host, accept,
        accept_encoding, content_type, content_length;
public:
    Request_Message(string cmd, string url);
    ~Request_Message(){}
    void addInfo(string h, string a, string encoding, string type);
    void setContent(time_t timestamp, string content);
};

class Response_Message: public Message {
private:
    string CMD, remote_path, type, data, response_header,
        content_type, content_length, content_encoding;
    unsigned length;
public:
    Response_Message();
    ~Response_Message(){}
    void decode();
    string getCMD();
    string getPath();
    string getType();
    string getData();
    unsigned getLength();
    void addInfo(string h, string a, string encoding, string type);
    void setContent(time_t timestamp, string content);
    void decodeResponse();
};

#endif //IPK_MESSAGE_H
