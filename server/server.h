#ifndef SERVER_H
#define SERVER_H

#define QUIT_INPUT 'q'

#include "acceptor.h"
#include "game_loop.h"

class Server {
private:
    GameLoop game_loop;

public:
    explicit Server(const char* port): game_loop(port) {}

    void initiate_communication() {
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
