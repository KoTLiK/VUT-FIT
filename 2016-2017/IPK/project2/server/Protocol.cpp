//

#include "Protocol.hpp"

Protocol::Protocol() {
    message.clear();
}

bool Protocol::check_add(char *buffer) {
    message += string(buffer);
    return message.find('\n') != string::npos;
}

bool Protocol::check_add(string buffer) {
    message += buffer;
    return message.find('\n') != string::npos;
}

bool Protocol::check_add(char *buffer, unsigned bytes) {
    message += string(buffer, bytes);
    return message.find('\n') != string::npos;
}

string Protocol::get() {
    return message;
}

void Protocol::clear() {
    message.clear();
}
