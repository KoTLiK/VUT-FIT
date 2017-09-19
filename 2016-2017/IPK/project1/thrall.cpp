//
// Created by milan on 7.3.2017.
//

#include "CoreException.h"
#include "ARGS.h"
#include "Client.h"

using namespace std;

static const char *HELP_STRING = "Try: ./ftrest COMMAND REMOTE-PATH [LOCAL-PATH]\n";
bool proceed(C_ARGS *A, Message *M);

int main(int argc, char **argv) {
    C_ARGS Args;
    try { // Program settings
        Args.set(argc, argv);
    } catch (ArgsException &e) {
        cerr << "ERROR: " << e.what() << endl;
        cerr << HELP_STRING; exit(EXIT_FAILURE);
    }

    if (Args.getStatus()) {
        cerr << INFO_TYP[UNKNOWN] << endl;
        return EXIT_FAILURE;
    }

    // HTTP Header values & adjust Request
    Message *message = new Request_Message(Args.getCMD(), Args.getRemotePath());
    if (Args.getType() == "?type=file" && Args.getCMD() == "GET") {
            message->addInfo(Args.getHostName(), INFO_TYP[OCTET_STREAM], INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
            message->setContent(time(nullptr), "");
    } else if (Args.getType() == "?type=file" && Args.getCMD() == "PUT") {
        string line = "", cmd = "file -b --mime-type " + Args.getLocalPath(); // cmd for MIME-type
        char buff[64]; bzero(buff, 64);
        FILE *s = popen(cmd.c_str(), "r"); // Result in file stream
        while (fgets(buff, 63, s) != NULL) {
            line += string(buff);
            bzero(buff, 64);
        }
        pclose(s);
        if (line[line.size()-1] == '\n') // Aesthetic
            line = string(line, 0, line.size()-1);

        message->addInfo(Args.getHostName(), INFO_TYP[TEXT], INFO_TYP[IDENTITY], line);

        struct stat f_stat; FILE *file = NULL;
        if (stat(Args.getLocalPath().c_str(), &f_stat)) { // Get file stats
            cerr << INFO_TYP[FILENF] << endl;
            delete message;
            return EXIT_FAILURE;
        } else if (S_ISDIR(f_stat.st_mode)) { // Shall we open it?
            cerr << INFO_TYP[NFILE] << endl; // TODO Check error message
            delete message;
            return EXIT_FAILURE;
        }

        if ((file = fopen(Args.getLocalPath().c_str(), "rb")) == NULL) {
            delete message;
            printError("fopen() collapsed.", EXIT_FAILURE);
        }

        char *buffer = new char[f_stat.st_size+1]; // Content buffer
        bzero(buffer, (size_t)f_stat.st_size +1);

        if (fread(buffer, 1, (size_t)f_stat.st_size, file) != (size_t)f_stat.st_size) { // Fill buffer
            delete[] buffer;
            delete message;
            printError("fread() collapsed.", EXIT_FAILURE);
        }
        string content = string(buffer, (size_t)f_stat.st_size); // Convert, better manipulation & ZERO byte fix
        delete[] buffer; fclose(file);
        message->setContent(time(nullptr), content); // Prepare to send
    } else {
        message->addInfo(Args.getHostName(), INFO_TYP[TEXT], INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
        message->setContent(time(nullptr), "");
    }

    Client client;
    try { // Create connection to server and send + receive message
        client.setServer(Args.getHostName(), Args.getPort());
        client.makeConnection();
        client.sendMessage(message->blowUp());
        delete message;
        message = client.receiveMessage();
        message->decodeResponse();
        if (proceed(&Args, message)) {
            delete message;
            return EXIT_FAILURE;
        }
    } catch (ClientException &e) {
        delete message;
        printError(e.what(), EXIT_FAILURE);
    } catch (ArgsException &e) {
        delete message;
        printError(e.what(), EXIT_FAILURE);
    }

    delete message;
    return 0;
}


bool proceed(C_ARGS *A, Message *M) {
    if (M->getType() == "200 OK") {
        if (A->getCMD() == "GET" && A->getType() == "?type=file") {
            FILE *file = NULL;
            struct stat f_stat;
            bzero(&f_stat, sizeof(f_stat));
            if (stat(A->getLocalPath().c_str(), &f_stat)) { // Not a file/folder
                if ((file = fopen(A->getLocalPath().c_str(), "wb")) == NULL) {
                    cerr << INFO_TYP[UNKNOWN] << endl;
                    return true;
                } else if (fwrite(M->getData().c_str(), 1, M->getLength(), file) != M->getLength()) {
                    cerr << INFO_TYP[UNKNOWN] << endl;
                    return true;
                } else {
                    fclose(file);
                }
            } else if (S_ISDIR(f_stat.st_mode)) {
                const char *path = A->getRemotePath().c_str();
                unsigned long i = A->getRemotePath().size()-1;
                for (unsigned stage = 0; i > 0 || stage != 3; --i) {
                    switch (stage) {
                        case 0: if (path[i] == '?') {
                                path = string(A->getRemotePath(), 0, i).c_str();
                                stage = 1;
                            } break;
                        case 1: if (path[i] != '/') stage = 2;
                            break;
                        case 2: if (path[i] == '/') {
                                path = string(string(path), i).c_str();
                                stage = 3;
                            } break;
                    }
                }
                if ((file = fopen((A->getLocalPath()+string(path)).c_str(), "wb")) == NULL) {
                    cerr << INFO_TYP[UNKNOWN] << endl;
                    return true;
                } else if (fwrite(M->getData().c_str(), 1, M->getLength(), file) != M->getLength()) {
                    cerr << INFO_TYP[UNKNOWN] << endl;
                    return true;
                } else {
                    fclose(file);
                }
            } else if ((file = fopen(A->getLocalPath().c_str(), "wb")) == NULL) {
                    cerr << INFO_TYP[UNKNOWN] << endl;
                    return true;
                } else if (fwrite(M->getData().c_str(), 1, M->getLength(), file) != M->getLength()) {
                    cerr << INFO_TYP[UNKNOWN] << endl;
                    return true;
                } else {
                    fclose(file);
                }
        } else if (A->getCMD() == "GET" && A->getType() == "?type=folder") {
            if (M->getData().size() != 0)
                cout << M->getData();
            else cout << "";
        }
    } else {
        cerr << M->getType() << ": " << M->getData() << endl;
        return true;
    }
    return false;
}
