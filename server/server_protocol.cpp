#include "server_protocol.h"

#include <string>
#include <utility>
#include <vector>

#include "../common/constants.h"

ServerProtocol::ServerProtocol(Socket&& skt): skt(std::move(skt)) {}


// Recibir
ActionDTO ServerProtocol::receive_and_deserialize_action() {
    /*
     * Se delega la tarea de recibir el tipo de acción al SocketManager
     * Si este es válido, se traduce la acción completa a un ActionDTO y se devuelve
     * Caso contrario, se devuelve un ActionDTO vacío
     */
    uint8_t opcode;
    if (!skt_manager.receive_byte(skt, opcode)) {
        return {};
    }
    switch (opcode)
    {
    case CREATE_OPCODE:
        return receive_and_deserialize_game(ActionType::CREATE);
    case JOIN_OPCODE:
        return receive_and_deserialize_game(ActionType::JOIN);
    case LIST_OPCODE:
        return ActionDTO(ActionType::LIST);
    default:
        return {};
    }
}

ActionDTO ServerProtocol::receive_and_deserialize_game(const ActionType& action_type) {
    uint16_t size_game_name;
    if (!skt_manager.receive_two_bytes(skt, size_game_name)){
        return {};
    }

    std::vector<uint8_t> game_name(size_game_name);
    if (!skt_manager.receive_bytes(skt, game_name)){
        return {};
    }

    return {action_type, std::string(game_name.begin(), game_name.end())};
}

// Enviar

// Cerrar
void ServerProtocol::close() { skt_manager.close(skt); }