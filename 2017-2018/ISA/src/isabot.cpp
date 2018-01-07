//
// Created by Minko on 26.9.2017.
//

#include "common/common.h"
#include "config/Config.h"
#include "common/functions.h"
#include "client/Client.h"
#include "protocol/Controller.h"

Config *config;
Client bot;

void signalHandler(int) {
    try {
        bot.sendMessage("QUIT :I am shutting down, bye!" CRLF);
    } catch (BotException &e) {
        delete config;
        printError(e.what(), 1);
    }
    delete config;
    exit(0);
}

int main(int argc, char **argv) {
    Controller control;

    signal(SIGINT, signalHandler);

    config = arguments(argc, argv);

    if (config == nullptr || !config->check()) {
        delete config;
        printError(USAGE.c_str(), 1);
    }


    try {
        if (config->isHighlighting()) {
            control.setHighlighting(config->getHighlights());
            control.setSyslogAndConnect(config->getSyslogServer());
        }

        bot.setConnection(config->getHost(), config->getPort());
        bot.connectToServer();

        bot.sendMessage(NICK CRLF);
        bot.sendMessage(USER CRLF);
        bot.sendMessage("JOIN "+ config->getRawChannels() +CRLF);

        while (true) {
            bot.receiveMessage(&control);

            control.front(); // Prepares next message
            if (control.processMessage()) { // Message processing
                bot.sendMessage(control.getMessage());
            }
            control.pop();
        }
    } catch (BotException &e) {
        delete config;
        printError(e.what(), 1);
    }
}
