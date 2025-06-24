#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <vector>

#include "common/action_DTO.h"
#include "common/byte_converter.h"
#include "common/socket.h"
#include "common/socket_manager.h"

class ClientProtocol {
private:
    /* Configuración */
    Socket& skt;
    SocketManager skt_manager;
    ByteConverter byte_converter;

    /* Recepción */
    ObjectDTO deserialize_player(const std::vector<uint8_t>& data, size_t& i,
                                 const std::vector<uint16_t>& position);
    ObjectDTO deserialize_bombzone(const std::vector<uint8_t>& data, size_t& i,
                                   const std::vector<uint16_t>& position);
    ObjectDTO deserialize_obstacle(const std::vector<uint8_t>& data, size_t& i,
                                   const std::vector<uint16_t>& position);
    ObjectDTO deserialize_bomb(const std::vector<uint8_t>& data, size_t& i,
                               const std::vector<uint16_t>& position);
    ObjectDTO deserialize_bullet(size_t& i, const std::vector<uint16_t>& position);
    ObjectDTO deserialize_weapon(const std::vector<uint8_t>& data, size_t& i,
                                 const std::vector<uint16_t>& position);
    ActionDTO deserialize_update(std::vector<uint8_t>& data);
    ActionDTO deserialize_information(std::vector<uint8_t>& data);
    ActionDTO deserialize_configuration(std::vector<uint8_t>& data);
    ActionDTO deserialize_message(const std::vector<uint8_t>& data);
    ActionDTO deserialize_shop(std::vector<uint8_t>& data);
    ActionDTO deserialize_stats(std::vector<uint8_t>& data);

    /* Envío */
    void serialize_create(const ActionDTO& action, std::vector<uint8_t>& data);
    void serialize_join(const ActionDTO& action, std::vector<uint8_t>& data);
    void serialize_move(const ActionDTO& action, std::vector<uint8_t>& data);
    void serialize_shoot(const ActionDTO& action, std::vector<uint8_t>& data);
    void serialize_weapon(const ActionDTO& action, std::vector<uint8_t>& data);
    void serialize_ammo(const ActionDTO& action, std::vector<uint8_t>& data);
    void serialize_rotate(const ActionDTO& action, std::vector<uint8_t>& data);

public:
    /* Constructor */
    explicit ClientProtocol(Socket& skt);

    /* Recepción */
    ActionDTO receive_and_deserialize_action();

    /* Envío */
    bool serialize_and_send_action(const ActionDTO& action);

    /* Cierre */
    void kill();
};

#endif  // CLIENT_PROTOCOL_H
