#include "client_protocol.h"

#include <algorithm>
#include <string>
#include <vector>

#include "common/object_DTO.h"
#include "common/stats.h"

/* Constructor */
ClientProtocol::ClientProtocol(Socket& skt): skt(skt) {}

/* Recepción */
ObjectDTO ClientProtocol::deserialize_player(const std::vector<uint8_t>& data, size_t& i,
                                             const std::vector<uint16_t>& position) {
    std::vector<uint8_t> id(data.begin() + i + 5, data.begin() + i + 7);
    PlayerType player_type = static_cast<PlayerType>(data[i + 7]);
    PlayerSkin player_skin = static_cast<PlayerSkin>(data[i + 8]);
    WeaponModel weapon_model = static_cast<WeaponModel>(data[i + 9]);
    std::vector<uint8_t> health(data.begin() + i + 10, data.begin() + i + 12);
    std::vector<uint8_t> money(data.begin() + i + 12, data.begin() + i + 14);
    std::vector<uint8_t> ammo(data.begin() + i + 14, data.begin() + i + 16);
    std::vector<uint8_t> angle(data.begin() + i + 16, data.begin() + i + 20);
    ObjectDTO obj = {ObjectType::PLAYER,
                     position,
                     byte_converter.hex_big_endian_to_uint_16(id),
                     player_type,
                     player_skin,
                     weapon_model,
                     byte_converter.hex_big_endian_to_uint_16(health),
                     byte_converter.hex_big_endian_to_uint_16(money),
                     byte_converter.hex_big_endian_to_uint_16(ammo),
                     byte_converter.hex_big_endian_to_float(angle)};
    i += 20;
    return obj;
}

ObjectDTO ClientProtocol::deserialize_bombzone(const std::vector<uint8_t>& data, size_t& i,
                                               const std::vector<uint16_t>& position) {
    std::vector<uint8_t> width(data.begin() + i + 5, data.begin() + i + 7);
    std::vector<uint8_t> height(data.begin() + i + 7, data.begin() + i + 9);
    ObjectDTO obj = {ObjectType::BOMBZONE, position,
                     byte_converter.hex_big_endian_to_uint_16(width),
                     byte_converter.hex_big_endian_to_uint_16(height)};
    i += 9;
    return obj;
}

ObjectDTO ClientProtocol::deserialize_obstacle(const std::vector<uint8_t>& data, size_t& i,
                                               const std::vector<uint16_t>& position) {
    std::vector<uint8_t> width(data.begin() + i + 5, data.begin() + i + 7);
    std::vector<uint8_t> height(data.begin() + i + 7, data.begin() + i + 9);
    ObstacleType obstacle_type = static_cast<ObstacleType>(data[i + 9]);
    ObjectDTO obj = {ObjectType::OBSTACLE, position,
                     byte_converter.hex_big_endian_to_uint_16(width),
                     byte_converter.hex_big_endian_to_uint_16(height), obstacle_type};
    i += 10;
    return obj;
}

ObjectDTO ClientProtocol::deserialize_bomb(const std::vector<uint8_t>& data, size_t& i,
                                           const std::vector<uint16_t>& position) {
    std::vector<uint8_t> bomb_countdown(data.begin() + i + 5, data.begin() + i + 7);
    ObjectDTO obj = {ObjectType::BOMB, position,
                     byte_converter.hex_big_endian_to_uint_16(bomb_countdown)};
    i += 7;
    return obj;
}

ObjectDTO ClientProtocol::deserialize_bullet(size_t& i, const std::vector<uint16_t>& position) {
    ObjectDTO obj = {ObjectType::BULLET, position};
    i += 5;
    return obj;
}

ObjectDTO ClientProtocol::deserialize_weapon(const std::vector<uint8_t>& data, size_t& i,
                                             const std::vector<uint16_t>& position) {
    WeaponModel weapon_model = static_cast<WeaponModel>(data[i + 5]);
    ObjectDTO obj = {ObjectType::WEAPON, position, weapon_model};
    i += 6;
    return obj;
}

ActionDTO ClientProtocol::deserialize_information(std::vector<uint8_t>& data) {
    size_t match_count =
            static_cast<size_t>(byte_converter.hex_big_endian_to_uint_16({data[0], data[1]}));
    size_t map_count =
            static_cast<size_t>(byte_converter.hex_big_endian_to_uint_16({data[2], data[3]}));

    std::vector<std::string> matches;
    std::vector<std::string> maps;

    size_t i = 4;

    for (size_t j = 0; j < match_count; ++j) {
        uint16_t match_size = byte_converter.hex_big_endian_to_uint_16({data[i], data[i + 1]});
        i += 2;
        std::string match(data.begin() + i, data.begin() + i + match_size);
        matches.push_back(match);
        i += match_size;
    }

    for (size_t j = 0; j < map_count; ++j) {
        uint16_t map_size = byte_converter.hex_big_endian_to_uint_16({data[i], data[i + 1]});
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
            byte_converter.hex_big_endian_to_uint_16(id)};
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

        uint16_t id_value = byte_converter.hex_big_endian_to_uint_16(id);
        stats_out.kills[id_value] = byte_converter.hex_big_endian_to_uint_16(kills);
        stats_out.deaths[id_value] = byte_converter.hex_big_endian_to_uint_16(deaths);
        stats_out.money[id_value] = byte_converter.hex_big_endian_to_uint_16(money);
    }
    stats_out.last_winner = static_cast<PlayerType>(data[data.size() - 5]);
    std::vector<uint8_t> team_a_wins(data.end() - 3, data.end() - 1);
    std::vector<uint8_t> team_b_wins(data.end() - 1, data.end());
    stats_out.team_a_wins = byte_converter.hex_big_endian_to_uint_16(team_a_wins);
    stats_out.team_b_wins = byte_converter.hex_big_endian_to_uint_16(team_b_wins);
    return {ActionType::STATS, stats_out};
}

ActionDTO ClientProtocol::deserialize_message(const std::vector<uint8_t>& data) {
    WinnerTeamType winner_team_type = static_cast<WinnerTeamType>(data[data.size() - 1]);
    return {ActionType::MESSAGE, winner_team_type};
}

ActionDTO ClientProtocol::deserialize_update(std::vector<uint8_t>& data) {
    std::vector<ObjectDTO> objects;
    for (size_t i = 0; i < data.size();) {
        ObjectType object_type = static_cast<ObjectType>(data[i]);
        std::vector<uint8_t> x(data.begin() + i + 1, data.begin() + i + 3);
        std::vector<uint8_t> y(data.begin() + i + 3, data.begin() + i + 5);
        std::vector<uint16_t> position = {byte_converter.hex_big_endian_to_uint_16(x),
                                          byte_converter.hex_big_endian_to_uint_16(y)};
        switch (object_type) {
            case ObjectType::PLAYER:
                objects.push_back(deserialize_player(data, i, position));
                break;
            case ObjectType::BOMBZONE:
                objects.push_back(deserialize_bombzone(data, i, position));
                break;
            case ObjectType::OBSTACLE:
                objects.push_back(deserialize_obstacle(data, i, position));
                break;
            case ObjectType::BOMB:
                objects.push_back(deserialize_bomb(data, i, position));
                break;
            case ObjectType::BULLET:
                objects.push_back(deserialize_bullet(i, position));
                break;
            case ObjectType::WEAPON:
                objects.push_back(deserialize_weapon(data, i, position));
                break;
            default:
                ++i;
                break;
        }
    }
    return {ActionType::UPDATE, objects};
}

ActionDTO ClientProtocol::receive_and_deserialize_action() {
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size))
        return {};

    uint8_t type_code;
    if (!skt_manager.receive_byte(skt, type_code))
        return {};

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
            return ActionDTO(type);
        case ActionType::MESSAGE:
            return deserialize_message(data);
        case ActionType::SHOP:
            return deserialize_shop(data);
        case ActionType::STATS:
            return deserialize_stats(data);
        default:
            return {};
    }
}

/* Envío */
void ClientProtocol::serialize_create(const ActionDTO& action, std::vector<uint8_t>& data) {
    byte_converter.push_uint_16_to(action.match.size(), data);
    data.insert(data.end(), action.match.begin(), action.match.end());
    data.insert(data.end(), action.map.begin(), action.map.end());
    byte_converter.push_uint_16_to(action.number_terrorist, data);
    byte_converter.push_uint_16_to(action.number_counterterrorist, data);
    data.push_back(static_cast<uint8_t>(action.player_type));
    data.push_back(static_cast<uint8_t>(action.player_skin));
}

void ClientProtocol::serialize_join(const ActionDTO& action, std::vector<uint8_t>& data) {
    data.insert(data.end(), action.match.begin(), action.match.end());
    data.push_back(static_cast<uint8_t>(action.player_type));
    data.push_back(static_cast<uint8_t>(action.player_skin));
}

void ClientProtocol::serialize_move(const ActionDTO& action, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(action.direction));
}

void ClientProtocol::serialize_shoot(const ActionDTO& action, std::vector<uint8_t>& data) {
    byte_converter.push_uint_16_to(action.desired_position[0], data);
    byte_converter.push_uint_16_to(action.desired_position[1], data);
}

void ClientProtocol::serialize_weapon(const ActionDTO& action, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(action.weapon));
}

void ClientProtocol::serialize_ammo(const ActionDTO& action, std::vector<uint8_t>& data) {
    byte_converter.push_uint_16_to(action.ammo, data);
}

void ClientProtocol::serialize_rotate(const ActionDTO& action, std::vector<uint8_t>& data) {
    byte_converter.push_float_to(action.angle, data);
}

bool ClientProtocol::serialize_and_send_action(const ActionDTO& action) {
    uint8_t opcode = static_cast<uint8_t>(action.type);
    std::vector<uint8_t> data = {opcode};
    switch (action.type) {
        case ActionType::CREATE:
            serialize_create(action, data);
            break;
        case ActionType::JOIN:
            serialize_join(action, data);
            break;
        case ActionType::MOVE:
            serialize_move(action, data);
            break;
        case ActionType::SHOOT:
            serialize_shoot(action, data);
            break;
        case ActionType::WEAPON:
            serialize_weapon(action, data);
            break;
        case ActionType::AMMOPRIMARY:
        case ActionType::AMMOSECONDARY:
            serialize_ammo(action, data);
            break;
        case ActionType::ROTATE:
            serialize_rotate(action, data);
            break;
        case ActionType::BOMB:
        case ActionType::CHANGE:
        case ActionType::START:
        case ActionType::PICKUP:
        case ActionType::QUIT:
        case ActionType::HEALTHCHEAT:
        case ActionType::AMMOCHEAT:
        case ActionType::MONEYCHEAT:
        case ActionType::WINCHEAT:
            break;
        default:
            return false;
    }
    return skt_manager.send_two_bytes(skt, data.size()) && skt_manager.send_bytes(skt, data);
}

/* Cierre */
void ClientProtocol::kill() { skt_manager.kill(skt); }
