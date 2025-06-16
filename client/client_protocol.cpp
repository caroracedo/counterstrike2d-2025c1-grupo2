#include "client_protocol.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

#include "../common/object_DTO.h"
#include "../common/stats.h"

/* Constructor */
ClientProtocol::ClientProtocol(Socket& skt): skt(skt) {}

/* Private */
/* Recepción */
ActionDTO ClientProtocol::deserialize_update(std::vector<uint8_t>& data) {
    std::vector<ObjectDTO> objects;
    for (size_t i = 0; i < data.size();) {
        ObjectType object_type = static_cast<ObjectType>(data[i]);
        std::vector<uint8_t> x(data.begin() + i + 1, data.begin() + i + 3);
        std::vector<uint8_t> y(data.begin() + i + 3, data.begin() + i + 5);
        std::vector<uint16_t> position = {hex_big_endian_to_int_16(x), hex_big_endian_to_int_16(y)};

        switch (object_type) {
            case ObjectType::PLAYER: {
                std::vector<uint8_t> id(data.begin() + i + 5, data.begin() + i + 7);
                PlayerType player_type = static_cast<PlayerType>(data[i + 7]);
                WeaponModel weapon_model = static_cast<WeaponModel>(data[i + 8]);
                std::vector<uint8_t> money(data.begin() + i + 10, data.begin() + i + 12);
                std::vector<uint8_t> ammo(data.begin() + i + 12, data.begin() + i + 14);
                std::vector<uint8_t> angle(data.begin() + i + 14, data.begin() + i + 16);
                objects.push_back({object_type, position, hex_big_endian_to_int_16(id), player_type,
                                   weapon_model, data[i + 9], hex_big_endian_to_int_16(money),
                                   hex_big_endian_to_int_16(ammo),
                                   hex_big_endian_to_float_16(angle)});
                i += 16;
                break;
            }
            case ObjectType::BOMBZONE: {
                std::vector<uint8_t> width(data.begin() + i + 5, data.begin() + i + 7);
                std::vector<uint8_t> height(data.begin() + i + 7, data.begin() + i + 9);
                objects.push_back({object_type, position, hex_big_endian_to_int_16(width),
                                   hex_big_endian_to_int_16(height)});
                i += 9;
                break;
            }
            case ObjectType::OBSTACLE: {
                std::vector<uint8_t> width(data.begin() + i + 5, data.begin() + i + 7);
                std::vector<uint8_t> height(data.begin() + i + 7, data.begin() + i + 9);
                ObstacleType obstacle_type = static_cast<ObstacleType>(data[i + 9]);
                objects.push_back({object_type, position, hex_big_endian_to_int_16(width),
                                   hex_big_endian_to_int_16(height), obstacle_type});
                i += 10;
                break;
            }
            case ObjectType::BOMB: {
                std::vector<uint8_t> bomb_countdown(data.begin() + i + 5, data.begin() + i + 7);
                objects.push_back(
                        {object_type, position, hex_big_endian_to_int_16(bomb_countdown)});
                i += 7;
                break;
            }
            case ObjectType::BULLET: {
                objects.push_back({object_type, position});
                i += 5;
                break;
            }
            case ObjectType::WEAPON: {
                WeaponModel weapon_model = static_cast<WeaponModel>(data[i + 5]);
                objects.push_back({object_type, position, weapon_model});
                i += 6;
                break;
            }
            default:
                break;
        }
    }
    return {ActionType::UPDATE, objects};
}

ActionDTO ClientProtocol::deserialize_information(std::vector<uint8_t>& data) {
    size_t match_count = static_cast<size_t>(hex_big_endian_to_int_16({data[0], data[1]}));
    size_t map_count = static_cast<size_t>(hex_big_endian_to_int_16({data[2], data[3]}));

    std::vector<std::string> matches;
    std::vector<std::string> maps;

    size_t i = 4;

    for (size_t j = 0; j < match_count; ++j) {
        uint16_t match_size = hex_big_endian_to_int_16({data[i], data[i + 1]});
        i += 2;
        std::string match(data.begin() + i, data.begin() + i + match_size);
        matches.push_back(match);
        i += match_size;
    }

    for (size_t j = 0; j < map_count; ++j) {
        uint16_t map_size = hex_big_endian_to_int_16({data[i], data[i + 1]});
        i += 2;
        std::string map(data.begin() + i, data.begin() + i + map_size);
        maps.push_back(map);
        i += map_size;
    }

    return {ActionType::INFORMATION, matches, maps};
}

ActionDTO ClientProtocol::deserialize_configuration(std::vector<uint8_t>& data) {
    std::vector<uint8_t> id(data.begin() + 1, data.end());
    return {ActionType::CONFIGURATION, static_cast<TerrainType>(data[0]),
            hex_big_endian_to_int_16(id)};
}

ActionDTO ClientProtocol::deserialize_shop(std::vector<uint8_t>& data) {
    std::vector<WeaponModel> weapons;
    std::transform(data.begin(), data.end(), std::back_inserter(weapons),
                   [](uint8_t byte) { return static_cast<WeaponModel>(byte); });
    return {ActionType::SHOP, weapons};
}

ActionDTO ClientProtocol::deserialize_stats(std::vector<uint8_t>& data) {
    Stats stats_out;
    for (size_t i = 0; i < data.size(); i += 8) {
        std::vector<uint8_t> id(data.begin() + i, data.begin() + i + 2);
        std::vector<uint8_t> kills(data.begin() + i + 2, data.begin() + i + 4);
        std::vector<uint8_t> deaths(data.begin() + i + 4, data.begin() + i + 6);
        std::vector<uint8_t> money(data.begin() + i + 6, data.begin() + i + 8);

        uint16_t id_value = hex_big_endian_to_int_16(id);
        stats_out.kills[id_value] = hex_big_endian_to_int_16(kills);
        stats_out.deaths[id_value] = hex_big_endian_to_int_16(deaths);
        stats_out.money[id_value] = hex_big_endian_to_int_16(money);
    }
    stats_out.last_winner = static_cast<PlayerType>(data[data.size() - 5]);
    std::vector<uint8_t> team_a_wins(data.end() - 3, data.end() - 1);
    std::vector<uint8_t> team_b_wins(data.end() - 1, data.end());
    stats_out.team_a_wins = hex_big_endian_to_int_16(team_a_wins);
    stats_out.team_b_wins = hex_big_endian_to_int_16(team_b_wins);
    return {ActionType::STATS, stats_out};
}

/* Public */
/* Recepción */
ActionDTO ClientProtocol::receive_and_deserialize_action() {
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size))
        return {};

    uint8_t type_code;
    if (!skt_manager.receive_byte(skt, type_code))
        return {};

    // TURBIOOO
    std::vector<uint8_t> data;
    if (size > 1) {
        data.resize(size - 1);
        if (!skt_manager.receive_bytes(skt, data) || data.empty())
            return {};
    }

    ActionType type = static_cast<ActionType>(type_code);
    switch (type) {
        case ActionType::UPDATE:
            return deserialize_update(data);
        case ActionType::INFORMATION:
            return deserialize_information(data);
        case ActionType::CONFIGURATION:
            return deserialize_configuration(data);
        case ActionType::END:
        case ActionType::WAIT:
            return ActionDTO(type);
        case ActionType::SHOP:
            return deserialize_shop(data);
        case ActionType::STATS:
            return deserialize_stats(data);
        default:
            return {};
    }
}

/* Envío */
bool ClientProtocol::serialize_and_send_action(const ActionDTO& action) {
    uint8_t opcode = static_cast<uint8_t>(action.type);
    std::vector<uint8_t> data = {opcode};
    switch (action.type) {
        case ActionType::CREATE:
            push_hexa_to(int_16_to_hex_big_endian(action.match.size()), data);
            data.insert(data.end(), action.match.begin(), action.match.end());
            data.insert(data.end(), action.map.begin(), action.map.end());
            data.push_back(static_cast<uint8_t>(action.player_type));
            break;
        case ActionType::JOIN:
            data.insert(data.end(), action.match.begin(), action.match.end());
            data.push_back(static_cast<uint8_t>(action.player_type));
            break;
        case ActionType::MOVE:
            data.push_back(static_cast<uint8_t>(action.direction));
            break;
        case ActionType::SHOOT:
            push_hexa_to(int_16_to_hex_big_endian(action.desired_position[0]), data);
            push_hexa_to(int_16_to_hex_big_endian(action.desired_position[1]), data);
            break;
        case ActionType::WEAPON:
            data.push_back(static_cast<uint8_t>(action.weapon));
            break;
        case ActionType::AMMOPRIMARY:
        case ActionType::AMMOSECONDARY:
            push_hexa_to(int_16_to_hex_big_endian(action.ammo), data);
            break;
        case ActionType::BOMB:
        case ActionType::CHANGE:
        case ActionType::START:
        case ActionType::PICKUP:
            break;
        case ActionType::ROTATE:
            push_hexa_to(int_16_to_hex_big_endian(action.angle), data);
            break;
        default:
            return false;
    }
    return skt_manager.send_two_bytes(skt, data.size()) && skt_manager.send_bytes(skt, data);
}
