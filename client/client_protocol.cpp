#include "client_protocol.h"

#include <cstring>
#include <utility>
#include <vector>

#include "../common/constants.h"

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
    // Me debería llegar una tira de bytes con el siguiente formato: <type><posx><posy>
    // Si es así:
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size))
        return {};
    std::vector<uint8_t> data(size);
    if (!skt_manager.receive_bytes(skt, data) || data.empty())
        return {};

    ActionType type = static_cast<ActionType>(data[0]);
    // if (type != ActionType::UPDATE) {
    //     return {};
    // }

    // Debería convertir en un vector de objetos otra vez
    // std::vector<Object> result = {};
    // for (size_t i = 1; i + 1 < data.size(); i += 3) {
    //     result.push_back(Object(static_cast<ObjectType>(data[i]),
    //     hex_big_endian_to_int(subvector(data, i + 1, 2))));
    // }
    // return {ActionType::UPDATE, result};
    return {};
}
