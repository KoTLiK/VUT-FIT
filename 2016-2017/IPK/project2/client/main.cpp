//
// Created by milan on 5.4.2017.
//

#include "header.hpp"
#include "Client.hpp"


int main(int argc, char **argv) {
    string server_ip;
    arguments(argc, argv, &server_ip);

    Client client;
    Protocol message;
    message.setAnswer("HELLO "+md5_hash(LOGIN)+"\n");

    try {
        client.setConnection(server_ip);
        client.connectToServer();

        client.sendMessage(message.get()); // Send HELLO

        client.receiveMessage(&message); // Receive first Message
        message.parse();

        while (message.getType() != "BYE") {
            if (!message.status() && !message.solve())
                message.setAnswer(string("RESULT ERROR\n"));

            // End iteration with sending answer & get new one
            if (!message.status()) client.sendMessage(message.get());

            message.clear();
            client.receiveMessage(&message);
            message.parse();
        }

        // We are here to make last step... print a top SECRET !
        cout << message.getSecret();

    } catch (ClientException &e) {
        printError(e.what(), 1);
    }

    return 0;
}