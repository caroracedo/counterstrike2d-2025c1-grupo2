#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>
#include <utility>

#include "../common/monitor_game.h"
#include "../common/socket.h"
#include "../common/thread.h"

#include "client_handler.h"

class Acceptor: public Thread {
private:
    Socket server_socket;
    MonitorGame& monitor_game;
    std::list<ClientHandler*> client_handlers_list;

    void clear() {
        for (auto* client_handler: client_handlers_list) {
            client_handler->hard_kill();
            client_handler->join();
            delete client_handler;
        }
        client_handlers_list.clear();
    }

    void reap() {
        client_handlers_list.remove_if([](auto* client_handler) {
            bool is_dead = !(client_handler->is_alive());
            if (is_dead) {
                client_handler->join();
                delete client_handler;
            }
            return is_dead;
        });
    }

public:
    /*
     * Constructor.
     **/
    Acceptor(const char* port, MonitorGame& monitor_game):
            server_socket(port), monitor_game(monitor_game) {}

    /* Override */
    void run() override {
        while (should_keep_running()) {
            try {
                Socket new_client_socket = server_socket.accept();
                ClientHandler* new_client_handler =
                        new ClientHandler(std::move(new_client_socket), monitor_game);
                reap();
                client_handlers_list.push_back(new_client_handler);
                new_client_handler->start();
            } catch (...) {
                break;
            }
        }
        clear();
    }

    void stop() override {
        Thread::stop();
        server_socket.shutdown(2);  // Cierra lectura y escritura
        server_socket.close();
    }
};

#endif  // ACCEPTOR_H
