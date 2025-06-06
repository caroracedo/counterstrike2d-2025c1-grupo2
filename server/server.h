#ifndef SERVER_H
#define SERVER_H

#define QUIT_INPUT 'q'

#include "../common/config.h"

#include "match_manager.h"

class Server {
private:
    MatchManager match_manager;

public:
    explicit Server(const char* yaml_path): match_manager(yaml_path) {}

    void run() {
        match_manager.start();

        char input;
        do {
            std::cin >> input;
        } while (input != QUIT_INPUT);

        match_manager.stop();
        match_manager.join();
    }
};

#endif  // SERVER_H
