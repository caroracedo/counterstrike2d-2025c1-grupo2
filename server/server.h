#ifndef SERVER_H
#define SERVER_H

#define QUIT_INPUT 'q'

#include "../common/config.h"

#include "acceptor.h"
#include "game_loop.h"

class Server {
private:
    GameLoop game_loop;

public:
    explicit Server(const char* yaml_path): game_loop(yaml_path) {}

    void run() {
        game_loop.start();

        char input;
        do {
            std::cin >> input;
        } while (input != QUIT_INPUT);

        game_loop.stop();
        game_loop.join();
    }
};

#endif  // SERVER_H
