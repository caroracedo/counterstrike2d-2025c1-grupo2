#include "server_protocol.h"

#include <algorithm>
#include <string>

#include "common/stats.h"

/* Constructor */
ServerProtocol::ServerProtocol(Socket& skt, uint16_t id): skt(skt), id(id) {}

/* Envío */
void ServerProtocol::serialize_player(const ObjectDTO& obj, std::vector<uint8_t>& data) {
    byte_converter.push_uint_16_to(obj.id, data);
    data.push_back(static_cast<uint8_t>(obj.player_type));
    data.push_back(static_cast<uint8_t>(obj.player_skin));
    data.push_back(static_cast<uint8_t>(obj.weapon_model));
    byte_converter.push_uint_16_to(obj.health, data);
    byte_converter.push_uint_16_to(obj.money, data);
    byte_converter.push_uint_16_to(obj.ammo, data);
    byte_converter.push_float_to(obj.angle, data);
}

void ServerProtocol::serialize_bombzone(const ObjectDTO& obj, std::vector<uint8_t>& data) {
    byte_converter.push_uint_16_to(obj.width, data);
    byte_converter.push_uint_16_to(obj.height, data);
}

void ServerProtocol::serialize_obstacle(const ObjectDTO& obj, std::vector<uint8_t>& data) {
    byte_converter.push_uint_16_to(obj.width, data);
    byte_converter.push_uint_16_to(obj.height, data);
    data.push_back(static_cast<uint8_t>(obj.obstacle_type));
}

void ServerProtocol::serialize_bomb(const ObjectDTO& obj, std::vector<uint8_t>& data) {
    byte_converter.push_uint_16_to(obj.bomb_countdown, data);
}

void ServerProtocol::serialize_weapon(const ObjectDTO& obj, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(obj.weapon_model));
}

void ServerProtocol::serialize_and_send_update(const ActionDTO& action_dto,
                                               std::vector<uint8_t>& data) {
    for (const auto& obj: action_dto.objects) {
        data.push_back(static_cast<uint8_t>(obj.type));
        byte_converter.push_uint_16_to(obj.position[0], data);
        byte_converter.push_uint_16_to(obj.position[1], data);

        switch (obj.type) {
            case ObjectType::PLAYER:
                serialize_player(obj, data);
                break;
            case ObjectType::BOMBZONE:
                serialize_bombzone(obj, data);
                break;
            case ObjectType::OBSTACLE:
                serialize_obstacle(obj, data);
                break;
            case ObjectType::BOMB:
                serialize_bomb(obj, data);
                break;
            case ObjectType::WEAPON:
                serialize_weapon(obj, data);
                break;
            default:
                break;
        }
    }
}

void ServerProtocol::serialize_and_send_information(const ActionDTO& action_dto,
                                                    std::vector<uint8_t>& data) {
    byte_converter.push_uint_16_to(action_dto.matches.size(), data);
    byte_converter.push_uint_16_to(action_dto.maps.size(), data);
    for (const auto& match: action_dto.matches) {
        byte_converter.push_uint_16_to(match.size(), data);
        data.insert(data.end(), match.begin(), match.end());
    }
    for (const auto& map: action_dto.maps) {
        byte_converter.push_uint_16_to(map.size(), data);
        data.insert(data.end(), map.begin(), map.end());
    }
}

void ServerProtocol::serialize_and_send_configuration(const ActionDTO& action_dto,
                                                      std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(action_dto.terrain_type));
    byte_converter.push_uint_16_to(action_dto.id, data);
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
        byte_converter.push_uint_16_to(player_id, data);
        byte_converter.push_uint_16_to(kills, data);
        byte_converter.push_uint_16_to(stats.deaths.find(player_id)->second, data);
        byte_converter.push_uint_16_to(stats.money.find(player_id)->second, data);
    }
    data.push_back(static_cast<uint8_t>(stats.last_winner));
    byte_converter.push_uint_16_to(stats.team_a_wins, data);
    byte_converter.push_uint_16_to(stats.team_b_wins, data);
}

void ServerProtocol::serialize_and_send_message(const ActionDTO& action_dto,
                                                std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(action_dto.winner_team_type));
}

bool ServerProtocol::serialize_and_send_action(const ActionDTO& action_dto) {
    std::vector<uint8_t> data = {static_cast<uint8_t>(action_dto.type)};
    switch (action_dto.type) {
        case ActionType::UPDATE:
            serialize_and_send_update(action_dto, data);
            break;
        case ActionType::INFORMATION:
            serialize_and_send_information(action_dto, data);
            break;
        case ActionType::CONFIGURATION:
            serialize_and_send_configuration(action_dto, data);
            break;
        case ActionType::SHOP:
            serialize_and_send_shop(action_dto, data);
            break;
        case ActionType::STATS:
            serialize_and_send_stats(action_dto, data);
            break;
        case ActionType::MESSAGE:
            serialize_and_send_message(action_dto, data);
            break;
        case ActionType::END:
            break;
        default:
            return false;
    }
    return skt_manager.send_two_bytes(skt, data.size()) && skt_manager.send_bytes(skt, data);
}

/* Recepción */
ActionDTO ServerProtocol::deserialize_create(const std::vector<uint8_t>& data) {
    uint16_t match_size = byte_converter.hex_big_endian_to_uint_16({data[1], data[2]});
    return {ActionType::CREATE,
            std::string(data.begin() + 3, data.begin() + 3 + match_size),
            std::string(data.begin() + 3 + match_size, data.end() - 6),
            byte_converter.hex_big_endian_to_uint_16(
                    {data[data.size() - 6], data[data.size() - 5]}),
            byte_converter.hex_big_endian_to_uint_16(
                    {data[data.size() - 4], data[data.size() - 3]}),
            static_cast<PlayerType>(data[data.size() - 2]),
            static_cast<PlayerSkin>(data.back())};
}

ActionDTO ServerProtocol::deserialize_join(const std::vector<uint8_t>& data) {
    return {ActionType::JOIN, std::string(data.begin() + 1, data.end() - 2),
            static_cast<PlayerType>(data[data.size() - 2]), static_cast<PlayerSkin>(data.back())};
}

ActionDTO ServerProtocol::deserialize_move(const std::vector<uint8_t>& data) {
    return {ActionType::MOVE, static_cast<Direction>(data[1]), id};
}

ActionDTO ServerProtocol::deserialize_shoot(const std::vector<uint8_t>& data) {
    return {ActionType::SHOOT,
            {byte_converter.hex_big_endian_to_uint_16({data[1], data[2]}),
             byte_converter.hex_big_endian_to_uint_16({data[3], data[4]})},
            id};
}

ActionDTO ServerProtocol::deserialize_weapon(const std::vector<uint8_t>& data) {
    return {ActionType::WEAPON, static_cast<WeaponModel>(data[1]), id};
}

ActionDTO ServerProtocol::deserialize_ammo(const std::vector<uint8_t>& data,
                                           WeaponType weapon_type) {
    return {weapon_type == WeaponType::PRIMARY ? ActionType::AMMOPRIMARY :
                                                 ActionType::AMMOSECONDARY,
            byte_converter.hex_big_endian_to_uint_16({data[1], data[2]}), weapon_type, id};
}

ActionDTO ServerProtocol::deserialize_rotate(const std::vector<uint8_t>& data) {
    return {ActionType::ROTATE,
            byte_converter.hex_big_endian_to_float({data[1], data[2], data[3], data[4]}), id};
}

ActionDTO ServerProtocol::deserialize_simple(const ActionType type) { return {type, id}; }

ActionDTO ServerProtocol::receive_and_deserialize_action() {
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size))
        return {};
    std::vector<uint8_t> data(size);
    if (!skt_manager.receive_bytes(skt, data) || data.empty())
        return {};

    ActionType type = static_cast<ActionType>(data[0]);
    switch (type) {
        case ActionType::CREATE:
            return deserialize_create(data);
        case ActionType::JOIN:
            return deserialize_join(data);
        case ActionType::MOVE:
            return deserialize_move(data);
        case ActionType::SHOOT:
            return deserialize_shoot(data);
        case ActionType::WEAPON:
            return deserialize_weapon(data);
        case ActionType::AMMOPRIMARY:
            return deserialize_ammo(data, WeaponType::PRIMARY);
        case ActionType::AMMOSECONDARY:
            return deserialize_ammo(data, WeaponType::SECONDARY);
        case ActionType::ROTATE:
            return deserialize_rotate(data);
        case ActionType::BOMB:
        case ActionType::CHANGE:
        case ActionType::START:
        case ActionType::PICKUP:
        case ActionType::QUIT:
        case ActionType::HEALTHCHEAT:
        case ActionType::AMMOCHEAT:
        case ActionType::MONEYCHEAT:
        case ActionType::WINCHEAT:
            return deserialize_simple(type);
        default:
            return {};
    }
}

/* Cierre */
void ServerProtocol::kill() { skt_manager.kill(skt); }
