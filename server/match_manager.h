#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include <algorithm>
#include <filesystem>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/socket.h"
#include "../common/thread.h"

#include "client_handler.h"
#include "match.h"
#include "monitor_client_send_queues.h"

#define YAML_EXTENSION ".yaml"

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
    void initialize_match_resources(const std::string& match, const std::string& map);
    bool is_valid_match(const std::string& match);
    std::vector<std::string> get_matches() const {
        std::vector<std::string> matches_vector;
        matches_vector.reserve(matches.size());  // opcional pero eficiente
        std::transform(matches.begin(), matches.end(), std::back_inserter(matches_vector),
                       [](const auto& match) { return match.first; });
        return matches_vector;
    }
    std::vector<std::string> get_maps() const {
        std::vector<std::string> maps_vector;
        for (const auto& entry: std::filesystem::directory_iterator(MAPS_PATH)) {
            if (entry.is_regular_file() && entry.path().extension() == YAML_EXTENSION) {
                maps_vector.push_back(entry.path().filename().string());
            }
        }
        return maps_vector;
    }

public:
    explicit MatchManager(const char* yaml_path);
    void run() override;
    void stop() override;
};

#endif  // MATCH_MANAGER_H
