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
    void serialize_and_send_update(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_information(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_configuration(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_shop(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_stats(const ActionDTO& action_dto, std::vector<uint8_t>& data);

public:
    /* Constructor */
    ServerProtocol(Socket& skt, uint16_t id);

    /* Recepción */
    ActionDTO receive_and_deserialize_action();

    /* Envío */
    bool serialize_and_send_action(const ActionDTO& action_dto);
};

#endif  // SERVER_PROTOCOL_H
