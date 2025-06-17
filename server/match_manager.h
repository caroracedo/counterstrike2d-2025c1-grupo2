#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include <algorithm>
#include <filesystem>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/socket.h"
#include "common/thread.h"

#include "client_handler.h"
#include "match.h"
#include "monitor_client_send_queues.h"

#define YAML_EXTENSION ".yaml"

class MatchManager: public Thread {
private:
    /* Configuración */
    Config config;
    Socket server_socket;

    /* Clientes */
    uint16_t id = 0;
    std::list<ClientHandler*> client_handlers_list;
    std::shared_ptr<Queue<ActionDTO>> own_recv_queue;

    /* Partidas */
    std::unordered_map<std::string, std::shared_ptr<Queue<ActionDTO>>> shared_recv_queues;
    std::unordered_map<std::string, std::shared_ptr<MonitorClientSendQueues>>
            monitors_client_send_queues;
    std::unordered_map<std::string, std::shared_ptr<Match>> matches;

    /* Limpieza */
    void kill_client_handler(ClientHandler* client_handler);
    void clear();
    void reap();

    /* Inicialización */
    void initialize_match_resources(const std::string& match, const std::string& map);
    void initialize_client_handler();
    std::pair<ClientHandler*, std::shared_ptr<Queue<ActionDTO>>> create_and_start_client_handler();
    void setup_client_handler_in_match(ClientHandler* handler,
                                       std::shared_ptr<Queue<ActionDTO>>& send_queue,
                                       const ActionDTO& action);

    /* Validación */
    bool is_valid_match(const std::string& match) const;
    bool should_initialize_match(const ActionDTO& action) const;

    /* Getters */
    std::vector<std::string> get_matches() const;
    std::vector<std::string> get_maps() const;

public:
    /* Constructor */
    explicit MatchManager(const char* yaml_path);

    /* Override */
    void run() override;
    void stop() override;
};

#endif  // MATCH_MANAGER_H
