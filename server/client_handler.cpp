#include "client_handler.h"

#include <utility>

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

/* Override */
void ClientHandler::run() {
    protocol.serialize_and_send_action(ActionDTO(
            ActionType::INFORMATION, matches_monitor.list_matches(), map_loader.get_maps()));

    ActionDTO first_action = protocol.receive_and_deserialize_action();
    std::shared_ptr<Queue<ActionDTO>> new_recv_queue;
    std::shared_ptr<Queue<ActionDTO>> send_queue;
    TerrainType terrain_type;
    if (first_action.type == ActionType::CREATE) {
        std::tie(new_recv_queue, send_queue, terrain_type) = matches_monitor.create_match(
                first_action.match, first_action.map, first_action.player_type, first_action.id);
    } else if (first_action.type == ActionType::JOIN) {
        std::tie(new_recv_queue, send_queue, terrain_type) = matches_monitor.join_match(
                first_action.match, first_action.player_type, first_action.id);
    }

    if (new_recv_queue != nullptr && send_queue != nullptr) {
        protocol.serialize_and_send_action(ActionDTO(ActionType::CONFIGURATION, terrain_type, id));

        receiver.set_queue(new_recv_queue);
        sender.set_queue(send_queue);

        receiver.start();
        sender.start();
    }
}

bool ClientHandler::is_alive() const { return !stop_flag; }

/* Cierre */
void ClientHandler::hard_kill() {
    Thread::stop();

    try {
        client_socket.shutdown(2);  // Cierra lectura y escritura
    } catch (...) {}
    client_socket.close();

    receiver.stop();
    sender.stop();

    receiver.join();
    sender.join();
}
