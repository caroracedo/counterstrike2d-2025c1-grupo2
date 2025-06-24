#ifndef SERVER_H
#define SERVER_H

#include "acceptor.h"

class Server {
private:
    /* Configuración */
    Acceptor acceptor;

public:
    /* Constructor */
    explicit Server(const char* yaml_path);

    /* Override */
    void run();
};

#endif  // SERVER_H
