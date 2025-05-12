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
    Socket skt;
    SocketManager skt_manager;

    bool serialize_and_send(const uint8_t opcode);
    bool serialize_and_send(const uint8_t opcode, const std::string& data);
    bool serialize_and_send(const uint8_t opcode, const std::vector<uint8_t>& data);

public:
    explicit ClientProtocol(Socket&& skt);

    bool serialize_and_send_action(const MainMenuDTO& main_menu);

    bool serialize_and_send_action(const ActionDTO& action);

    std::string receive_and_deserialize_games_names();

    ActionDTO receive_and_deserialize_move() {
        uint8_t size;
        skt_manager.receive_byte(skt, size);

        std::vector<uint8_t> buffer(size);
        skt_manager.receive_bytes(skt, buffer);

        if (buffer[0] != static_cast<uint8_t>(ActionType::MOVE)) {
            return {};
        }
        std::vector<uint8_t> position(buffer.begin() + 1, buffer.end());
        return {ActionType::MOVE, position};
    }
};

#endif  // CLIENT_PROTOCOL_H
