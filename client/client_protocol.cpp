#include "client_protocol.h"

#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

#include "../common/constants.h"
#include "../common/object_DTO.h"

ClientProtocol::ClientProtocol(Socket& skt): skt(skt) {}

bool ClientProtocol::serialize_and_send_action(const ActionDTO& action) {
    // Esto se mantiene...
    uint8_t opcode = static_cast<uint8_t>(action.type);
    switch (action.type) {
        case ActionType::MOVE: {
            std::vector<uint8_t> data = {opcode, static_cast<uint8_t>(action.direction)};
            return skt_manager.send_two_bytes(skt, data.size()) &&
                   skt_manager.send_bytes(skt, data);
        }
        default:
            return false;
    }
}

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
    size_t i = 0;

    while (i + 7 <= data.size()) { // al menos 7 bytes para empezar
        ObjectType object_type = static_cast<ObjectType>(data[i]);

        if (i + 7 > data.size()) break; // para los 7 bytes base
        std::vector<uint8_t> x(data.begin() + i + 1, data.begin() + i + 3);
        std::vector<uint8_t> y(data.begin() + i + 3, data.begin() + i + 5);
        std::vector<uint8_t> id(data.begin() + i + 5, data.begin() + i + 7);
        std::vector<uint16_t> position = {
            hex_big_endian_to_int_16(x),
            hex_big_endian_to_int_16(y)
        };

        if (object_type == ObjectType::OBSTACLE) {
            if (i + 11 > data.size()) break; // verificar que haya 11 bytes para OBSTACLE
            std::vector<uint8_t> width(data.begin() + i + 7, data.begin() + i + 9);
            std::vector<uint8_t> height(data.begin() + i + 9, data.begin() + i + 11);
            objects.push_back({
                object_type,
                position,
                hex_big_endian_to_int_16(id),
                hex_big_endian_to_int_16(width),
                hex_big_endian_to_int_16(height)
            });
            i += 11;
        } else {
            objects.push_back({
                object_type,
                position,
                hex_big_endian_to_int_16(id)
            });
            i += 7;
        }
    }
    return {ActionType::UPDATE, objects};
}
