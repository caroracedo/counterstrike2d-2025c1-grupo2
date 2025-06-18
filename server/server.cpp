#include "server.h"

#include <iostream>

#define QUIT_INPUT 'q'

/* Constructor */
Server::Server(const char* yaml_path): acceptor(yaml_path) {}

/* Override */
void Server::run() {
    acceptor.start();

    char input;
    do {
        std::cin >> input;
    } while (input != QUIT_INPUT);

    acceptor.stop();
    acceptor.join();
}
