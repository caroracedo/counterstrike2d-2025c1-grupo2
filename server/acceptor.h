#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>
#include <utility>
#include <vector>

#include "../common/monitor_game.h"
#include "../common/socket.h"
#include "../common/thread.h"

#include "client_handler.h"

class Acceptor: public Thread {
private:
    Socket server_socket;
    Queue<ActionDTO>& shared_recv_queue;
    std::vector<Queue<ActionDTO>*>& client_send_queues;
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
        client_handlers_list.remove_if([](ClientHandler* client_handler) {
            if (!client_handler->is_alive()) {
                client_handler->join();
                delete client_handler;
                return true;
            }
            return false;
        });
    }

public:
    Acceptor(const char* port, Queue<ActionDTO>& shared_recv_queue,
             std::vector<Queue<ActionDTO>*>& send_queues):
            server_socket(port),
            shared_recv_queue(shared_recv_queue),
            client_send_queues(send_queues) {}

    void run() override {
        while (should_keep_running()) {
            try {
                Socket new_client_socket = server_socket.accept();
                Queue<ActionDTO>* send_queue = new Queue<ActionDTO>();
                client_send_queues.push_back(send_queue);
                ClientHandler* new_client_handler = new ClientHandler(
                        std::move(new_client_socket), shared_recv_queue, *send_queue);
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
