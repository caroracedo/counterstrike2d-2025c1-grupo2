#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <vector>

#include "common/action_DTO.h"
#include "common/byte_converter.h"
#include "common/socket.h"
#include "common/socket_manager.h"

class ServerProtocol {
private:
    /* Configuration */
    Socket& skt;
    SocketManager skt_manager;
    ByteConverter byte_converter;
    uint16_t id;

    /* Envío */
    void serialize_player(const ObjectDTO& obj, std::vector<uint8_t>& data);
    void serialize_bombzone(const ObjectDTO& obj, std::vector<uint8_t>& data);
    void serialize_obstacle(const ObjectDTO& obj, std::vector<uint8_t>& data);
    void serialize_bomb(const ObjectDTO& obj, std::vector<uint8_t>& data);
    void serialize_weapon(const ObjectDTO& obj, std::vector<uint8_t>& data);
    void serialize_and_send_update(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_information(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_configuration(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_shop(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_stats(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_message(const ActionDTO& action_dto, std::vector<uint8_t>& data);

    /* Recepción */
    ActionDTO deserialize_create(const std::vector<uint8_t>& data);
    ActionDTO deserialize_join(const std::vector<uint8_t>& data);
    ActionDTO deserialize_move(const std::vector<uint8_t>& data);
    ActionDTO deserialize_shoot(const std::vector<uint8_t>& data);
    ActionDTO deserialize_weapon(const std::vector<uint8_t>& data);
    ActionDTO deserialize_ammo(const std::vector<uint8_t>& data, WeaponType weapon_type);
    ActionDTO deserialize_rotate(const std::vector<uint8_t>& data);
    ActionDTO deserialize_simple(const ActionType type);

public:
    /* Constructor */
    ServerProtocol(Socket& skt, uint16_t id);

    /* Envío */
    bool serialize_and_send_action(const ActionDTO& action_dto);

    /* Recepción */
    ActionDTO receive_and_deserialize_action();

    /* Cierre */
    void kill();
};

#endif  // SERVER_PROTOCOL_H
