#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>
#include <utility>

#include "../common_src/monitor_games.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"

#include "client_manager.h"

class Acceptor: public Thread {
private:
    Socket server_socket;
    MonitorGames& monitor_games;
    std::list<ClientManager*> client_managers_list;

    void close_server_clients_orderly() {
        for (ClientManager* server_client: client_managers_list) {
            server_client->join();
            delete server_client;
        }
    }

    void start_server_client() {
        Socket new_client_socket = server_socket.accept();
        ClientManager* new_client_manager =
                new ClientManager(std::move(new_client_socket), monitor_games);
        new_client_manager->start();
        client_managers_list.push_back(new_client_manager);
    }

public:
    /*
     * Constructor.
     **/
    Acceptor(const char* port, MonitorGames& monitor_games):
            server_socket(port), monitor_games(monitor_games) {}

    /* Override */
    void run() override {
        while (should_keep_running()) {
            try {
                start_server_client();
            } catch (...) {
                break;
            }
        }
        close_server_clients_orderly();
    }

    void stop() override {
        Thread::stop();
        server_socket.shutdown(2);  // Cierra lectura y escritura
        server_socket.close();
    }
};

#endif  // ACCEPTOR_H
