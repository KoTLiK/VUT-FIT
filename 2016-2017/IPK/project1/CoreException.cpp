//
// Created by milan on 7.3.2017.
//

#include "CoreException.h"

void printError(const char *e, int code) {
    cerr << "ERROR: " << e << endl;
    exit(code);
}