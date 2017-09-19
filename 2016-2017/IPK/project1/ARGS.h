//
// Created by milan on 7.3.2017.
//

#ifndef IPK_S_ARGS_H
#define IPK_S_ARGS_H

#include "CoreException.h"

using namespace std;

/* *** class S_ARGS *** */

class S_ARGS {
private:
    string root_folder;
    long long int port;
public:
    S_ARGS();
    void set(int argc, char **argv);
    int getPort();
    const char *getRootFolder();
    ~S_ARGS(){}
};

/* *** class C_ARGS *** */

class C_ARGS {
private:
    string CMD, type, host_name, s_port, remote_path, local_path;
    bool err, machine(const char *fuel);
public:
    C_ARGS();
    void set(int argc, char **argv);
    string getCMD();
    string getType();
    string getHostName();
    unsigned short getPort();
    string getRemotePath();
    string getLocalPath();
    bool getStatus();
    ~C_ARGS(){}
};

#endif //IPK_S_ARGS_H
