#ifndef SERVER_H
#define SERVER_H

#include "match_manager.h"

class Server {
private:
    /* Configuración */
    MatchManager match_manager;

public:
    /* Constructor */
    explicit Server(const char* yaml_path);

    /* Override */
    void run();
};

#endif  // SERVER_H
