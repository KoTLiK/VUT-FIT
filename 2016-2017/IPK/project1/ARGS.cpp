//
// Created by milan on 7.3.2017.
//

#include "ARGS.h"

/* *** class S_ARGS *** */

S_ARGS::S_ARGS() {
    root_folder = "./";
    port = 6677;
}

int S_ARGS::getPort() {
    return (int) port;
}

const char *S_ARGS::getRootFolder() {
    return root_folder.c_str();
}

void S_ARGS::set(int argc, char **argv) {
    int c;
    if (argc > 5) throw ArgsException("Wrong usage!");
    while ((c = getopt(argc, argv, "r:p:")) != -1) {
        switch (c) {
            case 'r':
                if (optarg) root_folder = optarg;
                break;
            case 'p':
                if (optarg) {
                    char *endptr = NULL;
                    if (strlen(optarg) > 6)
                        throw ArgsException("Wrong format of port!");
                    port = strtoll(optarg, &endptr, 10);
                    if (*endptr != '\0' || port < 1024 || port > 65535)
                        throw ArgsException("Wrong format of port!");
                }
                break;
            default: throw ArgsException("Wrong usage!");
        }
    }

    struct stat s;
    if (stat(root_folder.c_str(),&s))
        throw ArgsException("Wrong ROOT-PATH!");
    else if (!S_ISDIR(s.st_mode))
        throw ArgsException("Not a ROOT-FOLDER!");
}

/* *** class C_ARGS *** */

C_ARGS::C_ARGS() {
    type = "?type=";
    host_name = "";
    s_port = "";
    remote_path = "";
    local_path = "./";
    err = false;
}

void C_ARGS::set(int argc, char **argv) {
    if (argc < 3 || argc > 4) throw ArgsException("Wrong usage!");
    string cmd = argv[1];
    if (cmd == "del") {
        CMD = "DELETE";
        type += "file";
    } else if (cmd == "get") {
        CMD = "GET";
        type += "file";
    } else if (cmd == "put" && argc == 4) {
        CMD = "PUT";
        type += "file";
    } else if (cmd == "lst") {
        CMD = "GET";
        type += "folder";
    } else if (cmd == "mkd") {
        CMD = "PUT";
        type += "folder";
    } else if (cmd == "rmd") {
        CMD = "DELETE";
        type += "folder";
    } else throw ArgsException("Unknown COMMAND!");

    string a = argv[2];
    if (a.find("http://"))
        throw ArgsException("Not a http request!");
    if (!machine(a.c_str()))
        throw ArgsException("Wrong REMOTE-PATH!");

    remote_path += type;
    if (argc == 4) local_path = argv[3];

    // Validation of local path
    const char *buff = local_path.c_str();
    unsigned stage, i, pos = 0;
    for (stage = 1, i = (unsigned)local_path.size()-1; 0 < i; --i) {
        switch (stage) {
            case 1: if (buff[i] == '/') stage = 2;
                else stage = 3;
                break;
            case 2: if (buff[i] != '/') stage = 3;
                break;
            case 3: if (buff[i] == '/') {
                    pos = i;
                    stage = 4;
                } break;
            default: break;
        }
    }

    string sub_path = string(local_path, 0, pos);
    struct stat st;
    if (sub_path.size()) {
        if (stat(sub_path.c_str(), &st)) err = true;
        else if (!S_ISDIR(st.st_mode)) err = true;
    }
}

string C_ARGS::getCMD() {
    return CMD;
}

string C_ARGS::getType() {
    return type;
}

string C_ARGS::getHostName() {
    return host_name;
}

unsigned short C_ARGS::getPort() {
    char *endptr = NULL;
    long long num;
    if (s_port.size() > 6)
        throw ArgsException("Wrong format of port!");
    num = strtoll(s_port.c_str(), &endptr, 10);
    if (*endptr != '\0' || num < 1024 || num > 65535)
        throw ArgsException("Wrong format of port!");
    return (unsigned short) num;
}

string C_ARGS::getRemotePath() {
    return remote_path;
}

string C_ARGS::getLocalPath() {
    return local_path;
}

bool C_ARGS::getStatus() {
    return err;
}

bool C_ARGS::machine(const char *fuel) {
    int stage = 0, counter = 0;
    for (int i = 0; fuel[i] != '\0'; ++i) {
        switch (stage) {
            case 0: if (fuel[i] == '/') counter++;
                if (counter == 2) stage = 1;
                break;
            case 1: if (fuel[i] != ':') host_name += fuel[i];
                else stage = 2;
                break;
            case 2: if (fuel[i] != '/') s_port += fuel[i];
                else {
                    remote_path += fuel[i];
                    stage = 3;
                } break;
            case 3: remote_path += fuel[i];
        }
    }
    return (stage == 3);
}
