//

#include "header.hpp"
#include "Server.hpp"

int main(int argc, char **argv) {
    string example_file, result_file;
    arguments(argc, argv, &example_file, &result_file);

    vector<string> examples;
    getContent(example_file, &examples);

    vector<string> results;
    getContent(result_file, &results);

    vector<string> hash_log;
    getContent("out", &hash_log);

    if (examples.size() != results.size())
        printError("Files format is wrong.", EXIT_FAILURE);

    Server server;

    try {
        server.start();
        int pid, socket;
        while (true) {
            server.openConnection();
            socket = server.getConnectionSocket();

            signal(SIGCHLD, SIG_IGN);
            pid = fork();

            if (pid < 0) {
                server.closeConnection(socket);
                server.stop();
                printError("fork() failed.", EXIT_FAILURE);
            }

            if (pid == 0) { // Child

                int child_pid = getpid();
                char str[INET6_ADDRSTRLEN];
                string login = to_string(child_pid);
                DEBUG_BLUE(login+": New Connection","");
                if (inet_ntop(AF_INET6, &(server.getClient()->sin6_addr), str, sizeof(str))) {
                    DEBUG_BLUE(login, "Client address is "<< str);
                    DEBUG_BLUE(login, "Client port is "<< ntohs(server.getClient()->sin6_port));
                }


                Protocol *message = NULL;
                try {
                    message = server.receiveMessage(socket);

                    string xx = message->get().substr(message->get().find(" ")+1);
                    xx = string(xx, 0, xx.size()-1);
                    find_login(&hash_log, &login, xx);

                    DEBUG_YELLOW(login, message->get()); // HELLO message expected :-)

                    int x = 0;
                    while (examples.size() > (unsigned)x) {
                        message->clear();
                        message->check_add(examples[x]);

                        server.sendMessage(message->get(), socket);
                        if (results[x].find("Ignore") == 0
                            || results[x].find("Bye...") == 0) {
                            x++;
                            continue; // Not a correct format => skip Receive (Client should not send anything)
                        }
                        delete message;
                        message = server.receiveMessage(socket);

                        // clog output for comparison
                        DEBUG_YELLOW(login, examples[x]
                                <<"\tClient: "<< message->get()
                                <<"\t  Ref.: "<< results[x]);

                        x++;
                    }
                    /*
                    message->clear();
                    message->check_add("BYE \033[0;31mBYE \033[0;0m\033[1;33m"+login+"\033[0;32m, nice to meet you!\033[0;0m\n");
                    server.sendMessage(message->get(), socket);
                    */
                } catch (ServerException &e) {
                    delete message;
                    server.closeConnection(socket);
                    printError(string("Child ID:"+ login +" died.").c_str(), EXIT_FAILURE);
                }


                DEBUG_BLUE("Bye "+login,str);

                delete message;
                server.closeConnection(socket);
                server.stop();
                exit(EXIT_SUCCESS);
            } else server.closeConnection(socket);
        }

    } catch (ServerException &e) {
        server.stop();
        printError(e.what(), EXIT_FAILURE);
    }

    server.stop();
    return 0;
}
