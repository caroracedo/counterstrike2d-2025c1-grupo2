#include "server.h"

#include <iostream>

#define QUIT_INPUT 'q'

/* Constructor */
Server::Server(const char* yaml_path): match_manager(yaml_path) {}

/* Override */
void Server::run() {
    match_manager.start();

    char input;
    do {
        std::cin >> input;
    } while (input != QUIT_INPUT);

    match_manager.stop();
    match_manager.join();
}
