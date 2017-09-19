//
// Created by milan on 7.3.2017.
//

#include "CoreException.h"
#include "ARGS.h"
#include "Message.h"
#include "Server.h"

using namespace std;

static const char *HELP_STRING = "Try: ./ftrestd [-r ROOT-FOLDER] [-p PORT]\n";
void proceed(S_ARGS *A, Message *M);
int validity(string root, string remote, struct stat *s);
int delEmptyDir(string dirname);
void scanDir(string dirname, string *content);

int main(int argc, char* argv[]) {
    S_ARGS Args;
    Server server;
    Message *msg;

    try {
        Args.set(argc, argv);
    } catch (ArgsException &e) {
        cerr << "ERROR: " << e.what() << endl;
        cerr << HELP_STRING; exit(EXIT_FAILURE);
    }

    server.setPort(Args.getPort());
    try {
        server.start();
        while (1) {
            server.openConnection();
            msg = server.receiveMessage();
            msg->decode();
            proceed(&Args, msg);
            server.sendMessage(msg->blowUp());
            server.closeConnection();
            delete msg;
        }
    } catch (ServerException &e) {
        printError(e.what(), EXIT_FAILURE);
    } catch (RecvMsgException &e) {
        printError(e.what(), EXIT_FAILURE);
    }

    return 0;
}


void proceed(S_ARGS *A, Message *M) {
    FILE *file = NULL;
    char *buffer = NULL;
    int x;
    struct stat f_stat;
    bzero(&f_stat, sizeof(f_stat));

    if ((x = validity(A->getRootFolder(), M->getPath(), &f_stat)) == -1) {
        //DEBUG_RED("Bad Request", "You failed.");
        M->addInfo(INFO_TYP[_400], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
        M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
    } else if (M->getType() == "file") { // FILE
        if (M->getCMD() == "PUT") { // Save File
            if (x > 0) {
                if (stat((A->getRootFolder() + M->getPath()).c_str(), &f_stat)) { // Get stats
                    if ((file = fopen((A->getRootFolder() + M->getPath()).c_str(), "wb")) == NULL) {
                        //DEBUG_RED("fopen()", "Collapsed");
                        M->addInfo(INFO_TYP[_500], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                    } else if (fwrite(M->getData().c_str(), 1, M->getLength(), file) != M->getLength()) {
                        //DEBUG_RED("fwrite()", "Collapsed");
                        M->addInfo(INFO_TYP[_500], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                    } else {
                        fclose(file);
                        M->addInfo(INFO_TYP[_200], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), "");
                    }
                } else if (S_ISREG(f_stat.st_mode) || S_ISDIR(f_stat.st_mode)) {
                    //DEBUG_RED("File/folder already exist", "You failed.");
                    M->addInfo(INFO_TYP[_409], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[EXIST]);
                }
            } else {
                if (x == 0) {
                    //DEBUG_RED("User folder", "Forbidden");
                    M->addInfo(INFO_TYP[_403], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                } else {
                    //DEBUG_RED("User account not found", "");
                    M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UACC]);
                }
            }
        } else if (M->getCMD() == "GET") { // Send File
            if (x > 0) {
                if (stat((A->getRootFolder() + M->getPath()).c_str(), &f_stat)) { // Get stats
                    //DEBUG_RED("File not exist", "You failed.");
                    M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[FILENF]);
                } else if (S_ISREG(f_stat.st_mode)) {
                    file = fopen((A->getRootFolder() + M->getPath()).c_str(), "rb");
                    buffer = new char[f_stat.st_size+1]; // Content buffer
                    bzero(buffer, (size_t)f_stat.st_size +1);
                    if (fread(buffer, 1, (size_t)f_stat.st_size, file) != (size_t)f_stat.st_size) {
                        //DEBUG_RED("fread()", "Collapsed");
                        delete[] buffer;
                        M->addInfo(INFO_TYP[_500], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                    } else {
                        string content = string(buffer, (size_t)f_stat.st_size); // Convert, better manipulation & ZERO byte fix
                        delete[] buffer;
                        fclose(file);
                        M->addInfo(INFO_TYP[_200], "", INFO_TYP[IDENTITY], INFO_TYP[OCTET_STREAM]);
                        M->setContent(time(nullptr), content);
                    }
                } else {
                    //DEBUG_RED("Not a File", "Send Error");
                    M->addInfo(INFO_TYP[_400], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[NFILE]);
                }
            } else {
                if (x == 0) {
                    //DEBUG_RED("User folder", "Forbidden");
                    M->addInfo(INFO_TYP[_403], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                } else {
                    //DEBUG_RED("User account not found", "");
                    M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UACC]);
                }
            }
        } else { // Delete File
            if (x > 0) {
                if (stat((A->getRootFolder() + M->getPath()).c_str(), &f_stat)) { // Get stats
                    //DEBUG_RED("File not exist", "You failed.");
                    M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[FILENF]);
                } else if (S_ISREG(f_stat.st_mode)) {
                    if (remove((A->getRootFolder() + M->getPath()).c_str())) {
                        //DEBUG_RED("remove()", "Collapsed");
                        M->addInfo(INFO_TYP[_500], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                    } else {
                        M->addInfo(INFO_TYP[_200], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), "");
                    }
                } else {
                    //DEBUG_RED("Not a File", "Send Error");
                    M->addInfo(INFO_TYP[_400], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[NFILE]);
                }
            } else {
                if (x == 0) {
                    //DEBUG_RED("User folder", "Forbidden");
                    M->addInfo(INFO_TYP[_403], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                } else {
                    //DEBUG_RED("User account not found", "");
                    M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UACC]);
                }
            }
        }
    } else { // FOLDER
        if (M->getCMD() == "PUT") { // Create Folder
            if (x > 0) {
                if (stat((A->getRootFolder() + M->getPath()).c_str(), &f_stat)) { // Get stats
                    if (mkdir((A->getRootFolder() + M->getPath()).c_str(), 0755)) {
                        //DEBUG_RED("mkdir()", "Collapsed");
                        M->addInfo(INFO_TYP[_500], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                    } else {
                        M->addInfo(INFO_TYP[_200], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), "");
                    }
                } else if (S_ISREG(f_stat.st_mode) || S_ISDIR(f_stat.st_mode)) {
                    //DEBUG_RED("File/folder already exist", "You failed.");
                    M->addInfo(INFO_TYP[_409], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[EXIST]);
                }
            } else {
                if (x == 0) {
                    //DEBUG_RED("User folder", "Forbidden");
                    M->addInfo(INFO_TYP[_403], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                } else {
                    //DEBUG_RED("User account not found", "");
                    M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UACC]);
                }
            }
        } else if (M->getCMD() == "GET") { // Scan Folder
            if (x >= 0) {
                if (stat((A->getRootFolder() + M->getPath()).c_str(), &f_stat)) { // Get stats
                    //DEBUG_RED("Folder not exist", "You failed.");
                    M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[DIRNF]);
                } else if (S_ISDIR(f_stat.st_mode)) {
                    string content = "";
                    scanDir((A->getRootFolder() + M->getPath()).c_str(), &content);
                    M->addInfo(INFO_TYP[_200], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), content);
                } else {
                    //DEBUG_RED("Not a Folder", "Send Error");
                    M->addInfo(INFO_TYP[_400], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[NDIR]);
                }
            } else {
                //DEBUG_RED("User account not found", "");
                M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                M->setContent(time(nullptr), INFO_TYP[UACC]);
            }
        } else { // Delete Folder
            if (x > 0) {
                if (stat((A->getRootFolder() + M->getPath()).c_str(), &f_stat)) { // Get stats
                    //DEBUG_RED("Folder not exist", "You failed.");
                    M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[DIRNF]);
                } else if (S_ISDIR(f_stat.st_mode)) {
                    if ((x = delEmptyDir((A->getRootFolder() + M->getPath()).c_str())) == 0) {
                        M->addInfo(INFO_TYP[_200], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), "");
                    } else if (x > 0) {
                        //DEBUG_RED("Folder is not empty", "");
                        M->addInfo(INFO_TYP[_403], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), INFO_TYP[DIRNE]);
                    } else {
                        //DEBUG_RED("rmdir()", "Collapsed");
                        M->addInfo(INFO_TYP[_500], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                        M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                    }
                } else {
                    //DEBUG_RED("Not a Folder", "Send Error");
                    M->addInfo(INFO_TYP[_400], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[NDIR]);
                }
            } else {
                if (x == 0) {
                    //DEBUG_RED("User folder", "Forbidden");
                    M->addInfo(INFO_TYP[_403], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UNKNOWN]);
                } else {
                    //DEBUG_RED("User account not found", "");
                    M->addInfo(INFO_TYP[_404], "", INFO_TYP[IDENTITY], INFO_TYP[TEXT]);
                    M->setContent(time(nullptr), INFO_TYP[UACC]);
                }
            }
        }
    }
}


int validity(string root, string remote, struct stat *s) {
    const char *buff = remote.c_str();
    unsigned stage, i, pos = 0;
    string user = "/";
    for (stage = i = 0; buff[i] != '\0'; ++i) {
        switch (stage) {
            case 0: if (buff[i] != '/') {
                    user += buff[i];
                    stage = 1;
                } break;
            case 1: if (buff[i] != '/') user += buff[i];
                else {
                    user += buff[i];
                    stage = 2;
                } break;
            case 2: if (buff[i] != '/') stage = 3;
                break;
            default: break;
        }
    }

    if (remote.find("/../") != string::npos || remote.find("/./") == 0) return -1; // Bad request
    if (remote.find("/..") == remote.size()-3) return -1;

    if (stage == 0) return -2; // User-acc not found
    if (stat((root+user).c_str(), s)) return -1; // Bad request
    else if (!S_ISDIR(s->st_mode)) return -2; // User-acc not found
    if (stage < 3) return 0; // Only user

    for (stage = 1, i = (unsigned)remote.size()-1; 0 < i; --i) {
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

    if (stat((root+string(remote, 0, pos)).c_str(), s)) return -1; // Bad request
    if (S_ISDIR(s->st_mode)) return 1; // Valid path
    return -1; // Bad request
}


int delEmptyDir(string dirname) {
    int x = 0;
    DIR *dir = opendir(dirname.c_str());
    while (readdir(dir) != NULL) x++;
    closedir(dir);
    if (x > 2) return 1; // Not Empty
    else if (rmdir(dirname.c_str())) return -1; // Error
    return 0; // OK
}


void scanDir(string dirname, string *content) {
    string cmd = "ls " + dirname;
    char buff[64]; bzero(buff, 64);
    FILE *f = popen(cmd.c_str(), "r");
    while (fgets(buff, 63, f) != NULL) {
        *content += string(buff);
        bzero(buff, 64);
    }
    pclose(f);
}
