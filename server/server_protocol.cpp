#include "server_protocol.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

ServerProtocol::ServerProtocol(Socket& skt): skt(skt) {}

ActionDTO ServerProtocol::receive_and_deserialize_action() {
    // Esto se mantiene...
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size))
        return {};
    std::vector<uint8_t> data(size);
    if (!skt_manager.receive_bytes(skt, data) || data.empty())
        return {};

    ActionType type = static_cast<ActionType>(data[0]);
    switch (type) {
        case ActionType::MOVE:
            return {type, static_cast<Direction>(data[1])};
        default:
            return {};
    }
}

bool ServerProtocol::serialize_and_send_updated_game(const ActionDTO& action_dto) {
    // En el action_dto, me debería llegar un vector de objetos
    // Si es así:
    // if (action_dto.type != ActionType::UPDATE) {
    //     return false;
    // }
    std::vector<uint8_t> data;
    // for (uint16_t i = 0; i < action_dto.objects.size(); ++i) {
    //     data.push_back(static_cast<uint8_t>(action_dto.objects[i].type));
    //     data.push_back(int_to_hex_big_endian(action_dto.objects[i].position[0]));
    //     data.push_back(int_to_hex_big_endian(action_dto.objects[i].position[1])); // Por el
    //     momento 1x1
    // }
    return skt_manager.send_two_bytes(skt, data.size()) && skt_manager.send_bytes(skt, data);
}
