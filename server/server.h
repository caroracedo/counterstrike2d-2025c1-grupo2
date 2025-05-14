#ifndef SERVER_H
#define SERVER_H

#define QUIT_INPUT 'q'

#include "acceptor.h"

class Server {
private:
    MonitorGames monitor_games;
    Acceptor acceptor;

public:
    /*
     * Constructor.
     **/
    explicit Server(const char* port): acceptor(port, monitor_games) {}

    /* Iniciar la comunicación */
    void initiate_communication() {
        acceptor.start();

        char input;
        do {
            std::cin >> input;
        } while (input != QUIT_INPUT);

        acceptor.stop();
        acceptor.join();
    }
};

#endif  // SERVER_H
