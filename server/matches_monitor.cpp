#include "matches_monitor.h"

#include <algorithm>

#include "common/constants.h"

/* Constructor */
MatchesMonitor::MatchesMonitor(Config& config): config(config) {}

/* Crear Partida */
std::tuple<std::shared_ptr<Queue<ActionDTO>>, std::shared_ptr<Queue<ActionDTO>>, TerrainType>
        MatchesMonitor::create_match(const std::string& new_match_name,
                                     const std::string& desired_map, uint16_t number_terrorist,
                                     uint16_t number_counterterrorist, PlayerType player_type,
                                     PlayerSkin player_skin, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    std::shared_ptr<Queue<ActionDTO>> shared_recv_queue = std::make_shared<Queue<ActionDTO>>();
    std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queue =
            std::make_shared<MonitorClientSendQueues>();
    std::shared_ptr<Match> new_match = std::make_shared<Match>(
            config, shared_recv_queue, monitor_client_send_queue,
            (MAPS_PATH + desired_map + YAML_EXTENSION), number_terrorist, number_counterterrorist);

    shared_recv_queues[new_match_name] = shared_recv_queue;
    monitors_client_send_queues[new_match_name] = monitor_client_send_queue;
    matches[new_match_name] = new_match;

    new_match->start();
    new_match->add_player(player_type, player_skin, id);

    return {shared_recv_queue, monitor_client_send_queue->add_queue_to(id),
            new_match->get_terrain()};
}

/* Unirse a Partida */
std::tuple<std::shared_ptr<Queue<ActionDTO>>, std::shared_ptr<Queue<ActionDTO>>, TerrainType>
        MatchesMonitor::join_match(const std::string& match_name, PlayerType player_type,
                                   PlayerSkin player_skin, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    if (matches.find(match_name) != matches.end()) {
        auto shared_recv_queue = shared_recv_queues[match_name];
        auto monitor_client_send_queue = monitors_client_send_queues[match_name];
        auto match = matches[match_name];

        match->add_player(player_type, player_skin, id);

        return {shared_recv_queue, monitor_client_send_queue->add_queue_to(id),
                match->get_terrain()};
    }
    return {nullptr, nullptr, TerrainType::UNKNOWN};
}

/* Listar Patidas */
std::vector<std::string> MatchesMonitor::list_matches() {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<std::string> matches_vector;
    for (const auto& match: matches) {
        if (!match.second->is_started())
            matches_vector.push_back(match.first);
    }
    return matches_vector;
}

/* Limpieza */
void MatchesMonitor::reap() {
    std::lock_guard<std::mutex> lock(mutex);
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

void MatchesMonitor::clear() {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& match: matches) {
        match.second->stop();
        match.second->join();
    }
    matches.clear();
    shared_recv_queues.clear();
    monitors_client_send_queues.clear();
}
