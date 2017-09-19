//
// Created by milan on 5.4.2017.
//

#include "header.hpp"

void printError(const char *e, int code) {
    cerr << "ERROR: "<< e << endl;
    exit(code);
}

void arguments(int argc, char **argv, string *s) {
    if (argc != 2) {
        cerr << "Usage: ./"<< argv[0] << " hostname\n hostname - IPv4 / IPv6 address" << endl;
        exit(1);
    }
    *s = string(argv[1]);
}

string md5_hash(string s) {
    unsigned char raw[MD5_DIGEST_LENGTH];
    MD5_CTX c;

    MD5_Init(&c);
    MD5_Update(&c, s.c_str(), s.size());
    MD5_Final(raw, &c);

    char str[33];
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        sprintf(str+i*2, "%02x", raw[i]);

    return string(str);
}
