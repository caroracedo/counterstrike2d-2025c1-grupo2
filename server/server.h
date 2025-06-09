#ifndef SERVER_H
#define SERVER_H

#include "match_manager.h"

class Server {
private:
    MatchManager match_manager;

public:
    explicit Server(const char* yaml_path);
    void run();
};

#endif  // SERVER_H
