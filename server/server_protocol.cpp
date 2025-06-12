#include "server_protocol.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include "../common/stats.h"

/* Constructor */
ServerProtocol::ServerProtocol(Socket& skt, uint16_t id): skt(skt), id(id) {}

/* Private */
/* Envío */
void ServerProtocol::serialize_and_send_update(const ActionDTO& action_dto,
                                               std::vector<uint8_t>& data) {
    for (uint16_t i = 0; i < action_dto.objects.size(); ++i) {
        data.push_back(static_cast<uint8_t>(action_dto.objects[i].type));
        push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].position[0]), data);
        push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].position[1]), data);
        switch (action_dto.objects[i].type) {
            case ObjectType::PLAYER:
                push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].id), data);
                data.push_back(static_cast<uint8_t>(action_dto.objects[i].player_type));
                data.push_back(static_cast<uint8_t>(action_dto.objects[i].weapon_model));
                data.push_back(static_cast<uint8_t>(action_dto.objects[i].health));
                push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].money), data);
                push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].ammo), data);
                break;
            case ObjectType::BOMBZONE:
                push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].width), data);
                push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].height), data);
                break;
            case ObjectType::OBSTACLE:
                push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].width), data);
                push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].height), data);
                data.push_back(static_cast<uint8_t>(action_dto.objects[i].obstacle_type));
                break;
            case ObjectType::BOMB:
                push_hexa_to(int_16_to_hex_big_endian(action_dto.objects[i].bomb_countdown), data);
                break;
            // case ObjectType::WEAPON:
            //     data.push_back(static_cast<uint8_t>(action_dto.objects[i].weapon_model));
            //     break;
            default:
                break;
        }
    }
}

void ServerProtocol::serialize_and_send_configuration(const ActionDTO& action_dto,
                                                      std::vector<uint8_t>& data) {
    push_hexa_to(int_16_to_hex_big_endian(action_dto.matches.size()), data);
    push_hexa_to(int_16_to_hex_big_endian(action_dto.maps.size()), data);
    for (const auto& match: action_dto.matches) {
        push_hexa_to(int_16_to_hex_big_endian(match.size()), data);
        data.insert(data.end(), match.begin(), match.end());
    }
    for (const auto& map: action_dto.maps) {
        push_hexa_to(int_16_to_hex_big_endian(map.size()), data);
        data.insert(data.end(), map.begin(), map.end());
    }
    push_hexa_to(int_16_to_hex_big_endian(action_dto.id), data);
}

void ServerProtocol::serialize_and_send_shop(const ActionDTO& action_dto,
                                             std::vector<uint8_t>& data) {
    std::transform(action_dto.weapons.begin(), action_dto.weapons.end(), std::back_inserter(data),
                   [](WeaponModel weapon) { return static_cast<uint8_t>(weapon); });
}

void ServerProtocol::serialize_and_send_stats(const ActionDTO& action_dto,
                                              std::vector<uint8_t>& data) {
    const Stats& stats = action_dto.stats;
    for (const auto& [player_id, kills]: stats.kills) {
        push_hexa_to(int_16_to_hex_big_endian(player_id), data);
        push_hexa_to(int_16_to_hex_big_endian(kills), data);
        push_hexa_to(int_16_to_hex_big_endian(stats.deaths.find(player_id)->second), data);
        push_hexa_to(int_16_to_hex_big_endian(stats.money.find(player_id)->second), data);
    }
    data.push_back(static_cast<uint8_t>(stats.last_winner));
    push_hexa_to(int_16_to_hex_big_endian(stats.team_a_wins), data);
    push_hexa_to(int_16_to_hex_big_endian(stats.team_b_wins), data);
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
        case ActionType::CREATE: {
            uint16_t match_size = hex_big_endian_to_int_16({data[1], data[2]});
            return {type, std::string(data.begin() + 3, data.begin() + 3 + match_size),
                    std::string(data.begin() + 3 + match_size, data.end() - 1),
                    static_cast<PlayerType>(data.back()), id};
        }
        case ActionType::JOIN:
            return {type, std::string(data.begin() + 1, data.end() - 1),
                    static_cast<PlayerType>(data.back()), id};
        case ActionType::MOVE:
            return {type, static_cast<Direction>(data[1]), id};  // Agrega el id del jugador...
        case ActionType::SHOOT:
            return {type,
                    {hex_big_endian_to_int_16({data[1], data[2]}),
                     hex_big_endian_to_int_16({data[3], data[4]})},
                    id};  // Agrega el id del jugador...
        case ActionType::WEAPON:
            return {type, static_cast<WeaponModel>(data[1]), id};  // Agrega el id del jugador...
        case ActionType::AMMOPRIMARY:
            return {type, hex_big_endian_to_int_16({data[1], data[2]}), WeaponType::PRIMARY,
                    id};  // Agrega el id del jugador...
        case ActionType::AMMOSECONDARY:
            return {type, hex_big_endian_to_int_16({data[1], data[2]}), WeaponType::SECONDARY,
                    id};  // Agrega el id del jugador...
        case ActionType::BOMB:
        case ActionType::CHANGE:
            // case ActionType::TAKE:
            return {type, id};  // Agrega el id del jugador...
        default:
            return {};
    }
}

/* Envío */
bool ServerProtocol::serialize_and_send_action(const ActionDTO& action_dto) {
    std::vector<uint8_t> data = {static_cast<uint8_t>(action_dto.type)};
    switch (action_dto.type) {
        case ActionType::UPDATE:
            serialize_and_send_update(action_dto, data);
            break;
        case ActionType::CONFIGURATION:
            serialize_and_send_configuration(action_dto, data);
            break;
        case ActionType::SHOP:
            serialize_and_send_shop(action_dto, data);
            break;
        case ActionType::END:
            break;
        default:
            return false;
    }
    return skt_manager.send_two_bytes(skt, data.size()) && skt_manager.send_bytes(skt, data);
}
