//

#include "header.hpp"

void printError(const char *e, int code) {
    cerr << "ERROR: "<< e << endl;
    exit(code);
}

void arguments(int argc, char **argv, string *s1, string *s2) {
    if (argc != 3) {
        cerr << "Usage: ./"<< argv[0] << " message-file result-file" << endl;
        exit(EXIT_FAILURE);
    }
    *s1 = string(argv[1]);
    *s2 = string(argv[2]);
}

void getContent(string filename, vector<string> *vec) {
    ifstream file (filename);
    string line;

    if (!file) printError(string("While opening a file "+filename).c_str(), EXIT_FAILURE);

    while (getline(file, line)) {
        if (line.find("\n") == string::npos)
            line.append("\n"); // Line endings
        vec->push_back(line);
    }

    file.close();
}

unsigned find_all(string src, string c) {
    unsigned long p = src.find(c, 0);
    unsigned count = 0;

    while (p != string::npos) {
        p = src.find(c, p+c.size());
        count++;
    }

    return count;
}

bool find_login(vector<string> *str, string *login, string hash) {
    string x;
    for (unsigned i = 0; i < str->size(); ++i) {
        if ((*str)[i].find(hash) == 0) {
            x = (*str)[i].substr((*str)[i].find(" ")+1);
            *login = string(x, 0, x.size()-1);
            return true;
        }
    }
    return false;
}
