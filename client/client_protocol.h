#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>
#include <vector>

#include "../common/action_DTO.h"
#include "../common/constants.h"
#include "../common/main_menu_DTO.h"
#include "../common/socket.h"
#include "../common/socket_manager.h"

/*
Clase que encapsula el protocolo de comunicación del Client con el Server.
Se encarga del envío y recepción de mensajes.
*/
class ClientProtocol {
private:
    Socket& skt;
    SocketManager skt_manager;

    bool serialize_and_send(const uint8_t opcode);
    bool serialize_and_send(const uint8_t opcode, const std::string& data);
    bool serialize_and_send(const uint8_t opcode, const std::vector<uint8_t>& data);

    // inicialmente para recibir nombre de las partidas pero se puede usar para cualquier string
    std::string receive_and_deserialize_string();

    // inicialmente para recibir la posicion actualizada del jugador pero se puede usar para
    // cualquier vector
    std::vector<uint8_t> receive_and_deserialize_vector();

public:
    explicit ClientProtocol(Socket& skt);

    bool serialize_and_send_action(const MainMenuDTO& main_menu);

    bool serialize_and_send_action(const ActionDTO& action);

    std::string receive_and_deserialize_list();

    ActionDTO receive_and_deserialize_updated_position();

    void close();
};

#endif  // CLIENT_PROTOCOL_H
