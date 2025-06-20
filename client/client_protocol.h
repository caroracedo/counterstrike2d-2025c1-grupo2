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
    ActionDTO deserialize_update(std::vector<uint8_t>& data);
    ActionDTO deserialize_information(std::vector<uint8_t>& data);
    ActionDTO deserialize_configuration(std::vector<uint8_t>& data);
    ActionDTO deserialize_end();
    ActionDTO deserialize_shop(std::vector<uint8_t>& data);
    ActionDTO deserialize_stats(std::vector<uint8_t>& data);

public:
    /* Constructor */
    explicit ClientProtocol(Socket& skt);

    /* Recepción */
    ActionDTO receive_and_deserialize_action();

    /* Envío */
    bool serialize_and_send_action(const ActionDTO& action);

    /* Cerrar */
    void kill() { skt_manager.kill(skt); }
};

#endif  // CLIENT_PROTOCOL_H
