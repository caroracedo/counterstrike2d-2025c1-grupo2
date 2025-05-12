#include "client_protocol.h"

#include <cstring>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "../common/constants.h"

explicit ClientProtocol::ClientProtocol(Socket&& skt): skt(std::move(skt)) {}

bool ClientProtocol::serialize_and_send_main_menu_action(const MainMenuDTO& main_menu) {
    /*
     * Envía al Server el comando seleccionado por el Client, pudiendo ser este:
     * Create, Join, o List
     */
    switch (main_menu.option) {
        case Option::CREATE:
            return serialize_and_send_game_name(CREATE_OPCODE, main_menu.game_name);
        case Option::JOIN:
            return serialize_and_send_game_name(JOIN_OPCODE, main_menu.game_name);
        case Option::LIST:
            return serialize_and_send_opcode(LIST_OPCODE);
        default:
            return false;
    }
}

bool ClientProtocol::serialize_and_send_game_name(const uint8_t opcode,
                                                  const std::string& game_name) {

    /*
     * Envía al Server el comando, el largo del nombre de la partida y el nombre
     *  de la partida
     */
    uint16_t length = htons(static_cast<uint16_t>(game_name.size()));

    return (skt_manager.send_byte(skt, opcode) && skt_manager.send_two_bytes(skt, length) &&
            skt_manager.send_text(skt, game_name));
}

bool ClientProtocol::serialize_and_send_opcode(const uint8_t opcode) {
    return skt_manager.send_byte(skt, opcode);
}

bool ClientProtocol::serialize_and_send_action(const ActionDTO& action) {
    /**
     * Envía al Server la acción seleccionada por el Client, pudiendo ser esta:
     *  - Moverse en una dirección
     */
    switch (action.type) {
        case ActionType::MOVE:
            return serialize_and_send_move(action.direction);

        default:
            return false;
    }
}

bool ClientProtocol::serialize_and_send_move(const Direction direction) {
    /*
     * Envía al Server el comando MOVE y la dirección en la que se quiere mover
     */
    uint8_t direction_opcode;
    switch (direction) {
        case Direction::UP:
            direction_opcode = MOVE_UP_OPCODE;
            break;
        case Direction::DOWN:
            direction_opcode = MOVE_DOWN_OPCODE;
            break;
        case Direction::LEFT:
            direction_opcode = MOVE_LEFT_OPCODE;
            break;
        case Direction::RIGHT:
            direction_opcode = MOVE_RIGHT_OPCODE;
            break;
        default:
            return false;
    }

    std::vector<uint8_t> movement = {MOVE_OPCODE, direction_opcode};
    return skt_manager.send_bytes(skt, movement);
}

std::string ClientProtocol::receive_and_deserialize_games_names() {
    /*
     * Recibe del Server el mensaje con los nombres de las partidas actuales
     * y lo deserializa a un string.
     */
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size)) {
        return {};
    }

    std::vector<uint8_t> buffer(size);
    if (!skt_manager.receive_bytes(skt, buffer)) {
        return {};
    }

    std::string games_names(buffer.begin(), buffer.end());
    return games_names;
}
