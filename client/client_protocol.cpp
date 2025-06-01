#include "client_protocol.h"

#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

#include "../common/constants.h"
#include "../common/object_DTO.h"

ClientProtocol::ClientProtocol(Socket& skt): skt(skt) {}

bool ClientProtocol::serialize_and_send_action(const ActionDTO& action) {
    uint8_t opcode = static_cast<uint8_t>(action.type);
    std::vector<uint8_t> data = {opcode};
    switch (action.type) {
        case ActionType::MOVE: {
            data.push_back(static_cast<uint8_t>(action.direction));
            return skt_manager.send_two_bytes(skt, data.size()) &&
                   skt_manager.send_bytes(skt, data);
        }
        case ActionType::SHOOT: {
            push_hexa_to(int_16_to_hex_big_endian(action.desired_position[0]), data);
            push_hexa_to(int_16_to_hex_big_endian(action.desired_position[1]), data);
            return skt_manager.send_two_bytes(skt, data.size()) &&
                   skt_manager.send_bytes(skt, data);
        }
        default:
            return false;
    }
}

// TODO: Esto está RE largo, perdón por eso
ActionDTO ClientProtocol::receive_and_deserialize_updated_position() {
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size))
        return {};

    uint8_t type_code;
    if (!skt_manager.receive_byte(skt, type_code))
        return {};

    std::vector<uint8_t> data(size - 1);
    if (!skt_manager.receive_bytes(skt, data) || data.empty())
        return {};

    ActionType type = static_cast<ActionType>(type_code);
    if (type != ActionType::UPDATE)
        return {};

    std::vector<ObjectDTO> objects;
    for (size_t i = 0; i < data.size();) {
        ObjectType object_type = static_cast<ObjectType>(data[i]);
        std::vector<uint8_t> x(data.begin() + i + 1, data.begin() + i + 3);
        std::vector<uint8_t> y(data.begin() + i + 3, data.begin() + i + 5);
        std::vector<uint16_t> position = {hex_big_endian_to_int_16(x), hex_big_endian_to_int_16(y)};

        if (object_type == ObjectType::PLAYER) {
            std::vector<uint8_t> id(data.begin() + i + 5, data.begin() + i + 7);
            PlayerType player_type = static_cast<PlayerType>(data[i + 7]);
            WeaponModel weapon_model = static_cast<WeaponModel>(data[i + 8]);
            std::vector<uint8_t> money(data.begin() + i + 10, data.begin() + i + 12);
            objects.push_back({object_type, position, hex_big_endian_to_int_16(id), player_type,
                               weapon_model, data[i + 9], hex_big_endian_to_int_16(money)});
            i += 12;
        } else if (object_type == ObjectType::OBSTACLE) {
            std::vector<uint8_t> width(data.begin() + i + 5, data.begin() + i + 7);
            std::vector<uint8_t> height(data.begin() + i + 7, data.begin() + i + 9);
            objects.push_back({object_type, position, hex_big_endian_to_int_16(width),
                               hex_big_endian_to_int_16(height)});
            i += 9;
        } else {  // ObjectType::BULLET
            objects.push_back({object_type, position});
            i += 5;
        }
    }
    return {ActionType::UPDATE, objects};
}
