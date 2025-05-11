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
    uint16_t length = htons(static_cast<uint16_t>(game_name.size()));

    std::vector<uint8_t> message;
    message.push_back(opcode);
    message.push_back(static_cast<uint8_t>(length >> 8));
    message.push_back(static_cast<uint8_t>(length & 0xFF));
    message.insert(message.end(), game_name.begin(), game_name.end());

    /*
    Sugiero hacer:
    skt_manager.send_byte(game_name.size());
    skt_manager.send_two_bytes(length);
    std::vector<uint8_t> message(game_name.begin(), game_name.end());
    Así no se manejan bits a mano
    */

    return skt_manager.send_bytes(skt, message);
}

bool ClientProtocol::serialize_and_send_opcode(const uint8_t opcode) {
    return skt_manager.send_bytes(
            skt, {opcode});  // Y acá send_byte así no se crea un vector para un solo elemento
}

bool ClientProtocol::serialize_and_send_action(const ActionDTO& action) {
    switch (action.type) {
        case ActionType::MOVE:
            return serialize_and_send_move(action.direction);

        default:
            return false;
    }
}

bool ClientProtocol::serialize_and_send_move(const Direction direction) {
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

    std::vector<uint8_t> message = {MOVE_OPCODE, direction_opcode};
    return skt_manager.send_bytes(skt, message);
}

std::string ClientProtocol::receive_and_deserialize_games_names() {}
