#ifndef MATCHES_MONITOR_H
#define MATCHES_MONITOR_H

#include <algorithm>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "common/config.h"

#include "match.h"

class MatchesMonitor {
private:
    /* Configuración */
    Config& config;
    std::unordered_map<std::string, std::shared_ptr<Queue<ActionDTO>>> shared_recv_queues;
    std::unordered_map<std::string, std::shared_ptr<MonitorClientSendQueues>>
            monitors_client_send_queues;
    std::unordered_map<std::string, std::shared_ptr<Match>> matches;
    std::mutex mutex;

public:
    /* Constructor */
    explicit MatchesMonitor(Config& config);

    /* Crear Partida */
    std::tuple<std::shared_ptr<Queue<ActionDTO>>, std::shared_ptr<Queue<ActionDTO>>, TerrainType>
            create_match(const std::string& new_match_name, const std::string& desired_map,
                         uint16_t number_terrorist, uint16_t number_counterterrorist,
                         PlayerType player_type, PlayerSkin player_skin, uint16_t id);

    /* Unirse a Partida */
    std::tuple<std::shared_ptr<Queue<ActionDTO>>, std::shared_ptr<Queue<ActionDTO>>, TerrainType>
            join_match(const std::string& match_name, PlayerType player_type,
                       PlayerSkin player_skin, uint16_t id);

    /* Listar Patidas */
    std::vector<std::string> list_matches();

    /* Limpieza */
    void reap();
    void clear();
};

#endif  // MATCHES_MONITOR_H
