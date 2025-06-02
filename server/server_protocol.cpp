#include "server_protocol.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

/* Constructor */
ServerProtocol::ServerProtocol(Socket& skt, uint16_t id): skt(skt), id(id) {}

/* Private */
/* Envío */
bool ServerProtocol::serialize_and_send_update(const ActionDTO& action_dto,
                                               std::vector<uint8_t>& data) {
    for (uint16_t i = 0; i < action_dto.objects.size(); ++i) {
        data.push_back(static_cast<uint8_t>(action_dto.objects[i].type));
        push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].position[0]), data);
        push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].position[1]), data);
        if (action_dto.objects[i].type == ObjectType::PLAYER) {
            push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].id), data);
            data.push_back(static_cast<uint8_t>(action_dto.objects[i].player_type));
            data.push_back(static_cast<uint8_t>(action_dto.objects[i].weapon_model));
            data.push_back(static_cast<uint8_t>(action_dto.objects[i].health));
            push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].money), data);
        } else if (action_dto.objects[i].type == ObjectType::OBSTACLE) {
            push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].width), data);
            push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].height), data);
        }
    }
    return skt_manager.send_two_bytes(skt, data.size()) && skt_manager.send_bytes(skt, data);
}

bool ServerProtocol::serialize_and_send_id(const ActionDTO& action_dto,
                                           std::vector<uint8_t>& data) {
    push_hexa_to(int_16_to_hex_big_endian(action_dto.id), data);
    return skt_manager.send_two_bytes(skt, data.size()) && skt_manager.send_bytes(skt, data);
}

/* Public */
/* Recepción */
ActionDTO ServerProtocol::receive_and_deserialize_action() {
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size))
        return {};
    std::vector<uint8_t> data(size);
    if (!skt_manager.receive_bytes(skt, data) || data.empty())
        return {};

    ActionType type = static_cast<ActionType>(data[0]);
    switch (type) {
        case ActionType::PLAYERTYPE:
            return {type, static_cast<PlayerType>(data[1]), id};  // Agrega el id del jugador...
        case ActionType::MOVE:
            return {type, static_cast<Direction>(data[1]), id};  // Agrega el id del jugador...
        case ActionType::SHOOT: {
            return {type,
                    {hex_big_endian_to_int_16({data[1], data[2]}),
                     hex_big_endian_to_int_16({data[3], data[4]})},
                    id};  // Agrega el id del jugador...
        }
        default:
            return {};
    }
}

/* Envío */
bool ServerProtocol::serialize_and_send_action(const ActionDTO& action_dto) {
    std::vector<uint8_t> data = {static_cast<uint8_t>(action_dto.type)};
    if (action_dto.type == ActionType::UPDATE) {
        return serialize_and_send_update(action_dto, data);
    } else if (action_dto.type == ActionType::PLAYERID) {
        return serialize_and_send_id(action_dto, data);
    }
    return false;
}
