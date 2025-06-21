#include "client_handler.h"

/* Constructor */
ClientHandler::ClientHandler(Socket&& socket, MatchesMonitor& matches_monitor,
                             MapLoader& map_loader, uint16_t id):
        client_socket(std::move(socket)),
        protocol(client_socket, id),
        receiver(protocol, stop_flag),
        sender(protocol, stop_flag),
        matches_monitor(matches_monitor),
        map_loader(map_loader),
        id(id) {}

/* Lobby */
std::pair<bool, std::pair<std::shared_ptr<Queue<ActionDTO>>, std::shared_ptr<Queue<ActionDTO>>>>
        ClientHandler::lobby() {
    if (!protocol.serialize_and_send_action(ActionDTO(
                ActionType::INFORMATION, matches_monitor.list_matches(), map_loader.get_maps())))
        return {false, {nullptr, nullptr}};

    ActionDTO first_action = protocol.receive_and_deserialize_action();

    std::shared_ptr<Queue<ActionDTO>> new_recv_queue;
    std::shared_ptr<Queue<ActionDTO>> send_queue;
    TerrainType terrain_type;
    if (first_action.type == ActionType::CREATE) {
        std::tie(new_recv_queue, send_queue, terrain_type) = matches_monitor.create_match(
                first_action.match, first_action.map, first_action.number_terrorist,
                first_action.number_counterterrorist, first_action.player_type,
                first_action.player_skin, id);
    } else if (first_action.type == ActionType::JOIN) {
        std::tie(new_recv_queue, send_queue, terrain_type) = matches_monitor.join_match(
                first_action.match, first_action.player_type, first_action.player_skin, id);
    }

    if (!new_recv_queue && !send_queue)
        return {false, {nullptr, nullptr}};

    if (!protocol.serialize_and_send_action(ActionDTO(ActionType::CONFIGURATION, terrain_type, id)))
        return {false, {nullptr, nullptr}};

    return {true, {new_recv_queue, send_queue}};
}

/* Override */
void ClientHandler::run() {
    auto lobby_result = lobby();
    if (lobby_result.first) {
        auto& [recv_queue, send_queue] = lobby_result.second;
        receiver.set_queue(recv_queue);
        sender.set_queue(send_queue);
        receiver.start();
        sender.start();
    }
}

bool ClientHandler::is_alive() const { return Thread::is_alive() || !stop_flag; }

/* Cierre */
void ClientHandler::hard_kill() {
    Thread::stop();

    try {
        client_socket.shutdown(2);  // Cierra lectura y escritura
    } catch (...) {}
    client_socket.close();

    if (stop_flag) {
        receiver.stop();
        sender.stop();

        receiver.join();
        sender.join();
    }
}
