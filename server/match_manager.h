#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "../common/monitor_game.h"
#include "../common/socket.h"
#include "../common/thread.h"

#include "client_handler.h"
#include "match.h"
#include "monitor_client_send_queues.h"

class MatchManager: public Thread {
private:
    Config config;
    Socket server_socket;
    std::list<ClientHandler*> client_handlers_list;
    uint16_t id = 0;

    std::shared_ptr<Queue<ActionDTO>> own_recv_queue = std::make_shared<Queue<ActionDTO>>();
    std::unordered_map<std::string, std::shared_ptr<Queue<ActionDTO>>> shared_recv_queues;
    std::unordered_map<std::string, std::shared_ptr<MonitorClientSendQueues>>
            monitors_client_send_queues;
    std::unordered_map<std::string, std::shared_ptr<Match>> matches;

    void kill_client_handler(ClientHandler* client_handler) {
        client_handler->hard_kill();
        client_handler->join();
        delete client_handler;
    }

    void clear() {
        for (auto* client_handler: client_handlers_list) {
            kill_client_handler(client_handler);
        }
        client_handlers_list.clear();

        for (auto& match: matches) {
            match.second->stop();
            match.second->join();
        }
        matches.clear();
        shared_recv_queues.clear();
        monitors_client_send_queues.clear();
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

        for (auto it = matches.begin(); it != matches.end();) {
            if (!it->second->is_alive()) {
                it->second->join();
                shared_recv_queues.erase(it->first);
                monitors_client_send_queues.erase(it->first);
                it = matches.erase(it);
            } else {
                ++it;
            }
        }
    }

    void initialize_match_resources(const std::string& match) {
        std::shared_ptr<Queue<ActionDTO>> shared_recv_queue = std::make_shared<Queue<ActionDTO>>();
        std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues =
                std::make_shared<MonitorClientSendQueues>();

        shared_recv_queues[match] = shared_recv_queue;
        monitors_client_send_queues[match] = monitor_client_send_queues;
        matches[match] =
                std::make_shared<Match>(config, shared_recv_queue, monitor_client_send_queues);
    }

    bool is_valid_match(const std::string& match) {
        return !(shared_recv_queues.count(match) == 0 ||
                 monitors_client_send_queues.count(match) == 0 || matches.count(match) == 0);
    }

public:
    explicit MatchManager(const char* yaml_path):
            config(yaml_path), server_socket(config.get_server_port().c_str()) {}

    void run() override {
        while (should_keep_running()) {
            try {
                Socket new_client_socket = server_socket.accept();

                ClientHandler* client_handler =
                        new ClientHandler(std::move(new_client_socket), own_recv_queue, ++id);
                client_handler->start();

                ActionDTO first_action;
                while (!own_recv_queue->try_pop(first_action)) {}

                bool have_to_initialize_match = first_action.type == ActionType::CREATE ||
                                                (first_action.type == ActionType::JOIN &&
                                                 !is_valid_match(first_action.match));

                if (have_to_initialize_match)
                    initialize_match_resources(first_action.match);

                std::shared_ptr<Queue<ActionDTO>> shared_recv_queue =
                        shared_recv_queues[first_action.match];
                std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues =
                        monitors_client_send_queues[first_action.match];
                std::shared_ptr<Match> match = matches[first_action.match];

                std::shared_ptr<Queue<ActionDTO>> client_send_queue =
                        monitor_client_send_queues->add_queue_to(first_action.id);

                client_handler->bind_queues(shared_recv_queue, client_send_queue);
                client_send_queue->push(ActionDTO(ActionType::PLAYERID, first_action.id));

                client_handlers_list.push_back(client_handler);

                if (have_to_initialize_match)
                    match->start();
                match->add_player(first_action);
                reap();
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

#endif  // MATCH_MANAGER_H
