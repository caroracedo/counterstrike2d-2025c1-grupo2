#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

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

    std::shared_ptr<Queue<ActionDTO>> own_recv_queue;
    std::unordered_map<std::string, std::shared_ptr<Queue<ActionDTO>>> shared_recv_queues;
    std::unordered_map<std::string, std::shared_ptr<MonitorClientSendQueues>>
            monitors_client_send_queues;
    std::unordered_map<std::string, std::shared_ptr<Match>> matches;

    void kill_client_handler(ClientHandler* client_handler);
    void clear();
    void reap();
    void initialize_match_resources(const std::string& match);
    bool is_valid_match(const std::string& match);

public:
    explicit MatchManager(const char* yaml_path);
    void run() override;
    void stop() override;
};

#endif  // MATCH_MANAGER_H
