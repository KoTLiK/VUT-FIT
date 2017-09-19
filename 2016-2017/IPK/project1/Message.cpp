//
// Created by milan on 7.3.2017.
//

#include "Message.h"

/* *** class Message *** */

Message::Message() {
    message = "";
}

void Message::buffUp(char *buffer) {
    message += string(buffer);
}

void Message::buffUp(string buffer) {
    message += buffer;
}

void Message::buffUp(char *buffer, unsigned bytes) {
    message += string(buffer, bytes);
}

string Message::blowUp() {
    return message;
}

/* *** class Request_Message *** */

Request_Message::Request_Message(string cmd, string url) {
    request_header = cmd +" "+ url +" HTTP/1.1\r\n";
    host = "Host: ";
    accept = "Accept: ";
    accept_encoding = "Accept-Encoding: ";
    content_type = "Content-Type: ";
    content_length = "Content-Length: ";
}

void Request_Message::addInfo(string h, string a, string encoding, string type) {
    host += h +"\r\n";
    accept += a +"\r\n";
    accept_encoding += encoding +"\r\n";
    content_type += type +"\r\n";
}

void Request_Message::setContent(time_t timestamp, string content) {
    char time[100];
    struct tm *tm = localtime(&timestamp);
    strftime(time, sizeof(time), "%a, %d %b %Y %H:%M:%S %Z", tm);
    this->buffUp(request_header + host +"Date: "+ string(time) +"\r\n"+
                 accept + accept_encoding + content_type + content_length +
                 to_string(content.size()) + "\r\n\r\n");
    this->buffUp(content);
}

/* *** class Response_Message *** */

Response_Message::Response_Message() {
    CMD = "";
    remote_path = "";
    type = "";
    response_header = "HTTP/1.1 ";
    content_type = "Content-Type: ";
    content_length = "Content-Length: ";
    content_encoding = "Accept-Encoding: ";
}

void Response_Message::decode() {
    int stage, i;
    string field = "", value = "";
    const char *buff = message.c_str();
    for (stage = i = 0; buff[i] != '\0'; ++i) {
        if (stage == -1) break;
        switch (stage) {
            case 0: if (buff[i] == ' ') stage = 1;
                else CMD += buff[i];
                break;
            case 1: if (buff[i] == '?') stage = 2;
                else remote_path += buff[i];
                break;
            case 2: if (buff[i] == '=') stage = 3; break;
            case 3: if (buff[i] == ' ') stage = 4;
                else type += buff[i];
                break;
            case 4: if (buff[i] == '\n') stage = 5; break;
            case 5: if (buff[i] == '\r') stage = -1;
                else if (buff[i] == ' ') stage = 6;
                else field += buff[i];
                break;
            case 6: if (buff[i] == '\r') stage = 7;
                else value += buff[i];
                break;
            case 7: stage = 5;
                if (field == "Content-Length:")
                    length = (unsigned)strtoll(value.c_str(), nullptr, 10);
                field = ""; value = ""; break;
        }
    }
    if (length) data = string(message, (unsigned)++i, length);
}

string Response_Message::getCMD() {
    return CMD;
}

string Response_Message::getPath() {
    return remote_path;
}

string Response_Message::getType() {
    return type;
}

string Response_Message::getData() {
    return data;
}

unsigned Response_Message::getLength() {
    return length;
}

void Response_Message::addInfo(string h, string a, string encoding, string type) {
    (void) a;
    response_header += h +"\r\n";
    content_type += type +"\r\n";
    content_encoding += encoding +"\r\n";
}

void Response_Message::setContent(time_t timestamp, string content) {
    message.clear();
    char time[100];
    struct tm *tm = localtime(&timestamp);
    strftime(time, sizeof(time), "%a, %d %b %Y %H:%M:%S %Z", tm);
    this->buffUp(response_header +"Date: "+ string(time) +"\r\n"+
                 content_type + content_length +
                 to_string(content.size()) +"\r\n"+
                 content_encoding +"\r\n");
    this->buffUp(content);
}

void Response_Message::decodeResponse() {
    int stage, i;
    string field = "", value = "";
    const char *buff = message.c_str();
    for (stage = i = 0; buff[i] != '\0'; ++i) {
        if (stage == -1) break;
        switch (stage) {
            case 0: if (buff[i] == ' ') stage = 1;
                break;
            case 1: if (buff[i] == '\r') stage = 4;
                else type += buff[i];
                break;
            case 4: if (buff[i] == '\n') stage = 5; break;
            case 5: if (buff[i] == '\r') stage = -1;
                else if (buff[i] == ' ') stage = 6;
                else field += buff[i];
                break;
            case 6: if (buff[i] == '\r') stage = 7;
                else value += buff[i];
                break;
            case 7: stage = 5;
                if (field == "Content-Length:")
                    length = (unsigned)strtoll(value.c_str(), nullptr, 10);
                field = ""; value = ""; break;
        }
    }
    if (length) data = string(message, (unsigned)++i, length);
}
