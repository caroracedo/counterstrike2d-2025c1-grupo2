#include "server_protocol.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

ServerProtocol::ServerProtocol(Socket& skt, int id): skt(skt), id(id) {}

ActionDTO ServerProtocol::receive_and_deserialize_action() {
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size))
        return {};
    std::vector<uint8_t> data(size);
    if (!skt_manager.receive_bytes(skt, data) || data.empty())
        return {};

    ActionType type = static_cast<ActionType>(data[0]);
    switch (type) {
        case ActionType::MOVE:
            return {type, id, static_cast<Direction>(data[1])};  // Agrega el id del jugador...
        default:
            return {};
    }
}

bool ServerProtocol::serialize_and_send_updated_game(const ActionDTO& action_dto) {
    if (action_dto.type != ActionType::UPDATE)
        return false;

    std::vector<uint8_t> data = {static_cast<uint8_t>(ActionType::UPDATE)};
    for (uint16_t i = 0; i < action_dto.objects.size(); ++i) {
        data.push_back(static_cast<uint8_t>(action_dto.objects[i].type));
        push_hexa_to(int_to_hex_big_endian(action_dto.objects[i].position[0]), data);
        push_hexa_to(int_to_hex_big_endian(action_dto.objects[i].position[1]), data);
        data.push_back(static_cast<uint8_t>(action_dto.objects[i].id));
    }
    return skt_manager.send_two_bytes(skt, data.size()) && skt_manager.send_bytes(skt, data);
}
