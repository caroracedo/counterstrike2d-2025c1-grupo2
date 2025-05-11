#include "server_protocol.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "../common/constants.h"

ServerProtocol::ServerProtocol(Socket&& skt): skt(std::move(skt)) {}


// Recibir
MainMenuDTO ServerProtocol::receive_and_deserialize_main_menu_action() {
    /*
     * Recibe comando del Main Menu (Create, Join, List) enviada por el Client
     * En caso de ser un comando, devuelve un MainMenuDTO con campos necesarios completos
     * Caso contrario, devuelve un MainMenuDTO vacío (option=UNKNOWN)
     */
    uint8_t opcode;
    if (!skt_manager.receive_byte(skt, opcode)) {
        return {};
    }
    switch (opcode) {
        case CREATE_OPCODE:
            return receive_and_deserialize_game_name(Option::CREATE);
        case JOIN_OPCODE:
            return receive_and_deserialize_game_name(Option::JOIN);
        case LIST_OPCODE:
            return MainMenuDTO(Option::LIST);
        default:
            return {};
    }
}

MainMenuDTO ServerProtocol::receive_and_deserialize_game_name(const Option& action_type) {
    /*
     * Recibe el largo del nombre de la partida y el nombre
     * Usado cuando el comando es Option::CREATE o Option::JOIN
     */
    uint16_t size_game_name;
    if (!skt_manager.receive_two_bytes(skt, size_game_name)) {
        return {};
    }

    std::vector<uint8_t> game_name(size_game_name);
    if (!skt_manager.receive_bytes(skt, game_name)) {
        return {};
    }

    return {action_type, std::string(game_name.begin(), game_name.end())};
}

// Enviar
bool ServerProtocol::serialize_and_send_games_names(const std::vector<std::string>& names) {
    /*
     * Recibe un vector con los nombres de las partidas actuales y se las envía al Client
     * con el formato adecuado.
     */
    std::ostringstream oss;
    oss << "Partidas:";

    for (const std::string& name: names) {
        oss << "\n - " << name;
    }
    oss << "\n";
    std::string matches = oss.str();

    uint16_t size = htons(static_cast<uint16_t>(matches.size()));

    return skt_manager.send_two_bytes(skt, size) && skt_manager.send_text(skt, matches);
}

// Manejo de acciones
ActionDTO ServerProtocol::receive_and_deserialize_action() {
    /*
     * Recibe el opcode de la acción enviada por el cliente y
     * lo deserializa a un ActionDTO. Si no reconoce la acción,
     * devuelve un ActionDTO vacío
     */
    uint8_t opcode;
    if (!skt_manager.receive_byte(skt, opcode)) {
        return {};
    }

    switch (opcode) {
        case MOVE_OPCODE:
            return receive_and_deserialize_move();

        default:
            return {};
    }
}

ActionDTO ServerProtocol::receive_and_deserialize_move() {
    /*
     * Recibe el opcode de la dirección y lo deserializa a un ActionDTO.
     */
    uint8_t direction;
    if (!skt_manager.receive_byte(skt, direction)) {
        return {};
    }

    switch (direction) {
        case MOVE_UP_OPCODE:
            return {ActionType::MOVE, Direction::UP};
        case MOVE_DOWN_OPCODE:
            return {ActionType::MOVE, Direction::DOWN};
        case MOVE_LEFT_OPCODE:
            return {ActionType::MOVE, Direction::LEFT};
        case MOVE_RIGHT_OPCODE:
            return {ActionType::MOVE, Direction::RIGHT};
        default:
            return {};
    }
}

bool ServerProtocol::serialize_and_send_updated_position(ActionDTO action_dto) {
    /**
     * Recibe un ActionDTO con la nueva posición del cliente y se lo envía
     */
    // TODO: Definir bien el tamaño del vector position -> alcanza con un uint8_t?
    std::vector<uint8_t> data;
    data.push_back(MOVE_OPCODE);
    data.insert(data.end(), action_dto.position.begin(), action_dto.position.end());
    return skt_manager.send_bytes(skt, data);
}

// Cerrar
void ServerProtocol::close() { skt_manager.close(skt); }
