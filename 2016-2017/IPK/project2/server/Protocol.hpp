//

#ifndef IPK_MATH_SERVER_PROTOCOL_HPP
#define IPK_MATH_SERVER_PROTOCOL_HPP


#include "header.hpp"

class Protocol {
private:
    string message;
public:
    Protocol();
    ~Protocol(){}
    bool check_add(char *buffer);
    bool check_add(string buffer);
    bool check_add(char *buffer, unsigned bytes);
    string get();
    void clear();
};


#endif //IPK_MATH_SERVER_PROTOCOL_HPP
