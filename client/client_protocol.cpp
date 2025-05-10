#include "client_protocol.h"

#include "../common/constants.h"

#include <arpa/inet.h>
#include <cstring>

explicit ClientProtocol::ClientProtocol(Socket&& skt) : skt(std::move(skt)) {}

bool ClientProtocol::serialize_and_send_main_menu_action(const MainMenuDTO& main_menu) {
    /*
     * Envía al Server el comando seleccionado por el Client, pudiendo ser este: 
     * Create, Join, o List
     */
    switch (main_menu.option)
    {
    case Option::CREATE :
        return serialize_and_send_game_name(CREATE_OPCODE, main_menu.game_name);
    case Option::JOIN : 
        return serialize_and_send_game_name(JOIN_OPCODE, main_menu.game_name);
    case Option::LIST :
        return serialize_and_send_opcode(LIST_OPCODE);
    default:
        return false;
    }

}

bool ClientProtocol::serialize_and_send_game_name(const uint8_t opcode, const std::string& game_name) {
    std::vector<uint8_t> message = {opcode};

    uint16_t length = htons(static_cast<uint16_t>(game_name.size()));
    std::memcpy(&message[1], &length, sizeof(length));

    std::memcpy(&message[3], game_name.data(), game_name.size());
    
    return skt_manager.send_bytes(skt, message);
}

bool ClientProtocol::serialize_and_send_opcode(const uint8_t opcode) {
    return skt_manager.send_bytes(skt, {opcode});
}

bool ClientProtocol::serialize_and_send_action(const ActionDTO& action) {
    switch (action.type)
    {
    case ActionType::MOVE :
        return serialize_and_send_move(action.direction);
    
    default:
        return false;
    }
}

bool ClientProtocol::serialize_and_send_move(const Direction direction) {
    uint8_t direction_opcode;
    switch (direction)
    {
    case Direction::UP :
        direction_opcode = MOVE_UP_OPCODE;
        break;
    case Direction::DOWN : 
        direction_opcode = MOVE_DOWN_OPCODE;
        break;
    case Direction::LEFT : 
        direction_opcode = MOVE_LEFT_OPCODE;
        break;
    case Direction::RIGHT : 
        direction_opcode = MOVE_RIGHT_OPCODE;
        break;
    default:
        return false;
    }
    
    std::vector<uint8_t> message = {MOVE_OPCODE, direction_opcode};
    return skt_manager.send_bytes(skt, message);
}

std::string ClientProtocol::receive_and_deserialize_games_names() {

}
