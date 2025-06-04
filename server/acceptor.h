#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>
#include <utility>

#include "../common/monitor_game.h"
#include "../common/socket.h"
#include "../common/thread.h"

#include "client_handler.h"
#include "monitor_client_send_queues.h"

class Acceptor: public Thread {
private:
    Socket server_socket;
    Queue<ActionDTO>& shared_recv_queue;
    MonitorClientSendQueues& monitor_client_send_queues;
    std::list<ClientHandler*> client_handlers_list;
    MonitorGame& monitor_game;
    uint16_t id = 0;

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
    Acceptor(Config& config, Queue<ActionDTO>& shared_recv_queue,
             MonitorClientSendQueues& monitor_client_send_queues, MonitorGame& monitor_game):
            server_socket(config.get_server_port().c_str()),
            shared_recv_queue(shared_recv_queue),
            monitor_client_send_queues(monitor_client_send_queues),
            monitor_game(monitor_game) {}

    void run() override {
        while (should_keep_running()) {
            try {
                Socket new_client_socket = server_socket.accept();
                Queue<ActionDTO>& new_client_send_queue =
                        monitor_client_send_queues.add_queue_to(id);
                ClientHandler* new_client_handler =
                        new ClientHandler(std::move(new_client_socket), shared_recv_queue,
                                          new_client_send_queue, ++id);
                new_client_send_queue.push(ActionDTO(ActionType::PLAYERID, id));  // Para la gráfica
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
        server_socket.shutdown(2);
        server_socket.close();
    }
};

#endif  // ACCEPTOR_H
