#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>

#include "../common/action_DTO.h"
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

    bool serialize_and_send_game_name(const uint8_t opcode, const std::string& game_name);
    bool serialize_and_send_opcode(const uint8_t opcode);

    bool serialize_and_send_move(const Direction direction);

public:
    explicit ClientProtocol(Socket&& skt);

    bool serialize_and_send_main_menu_action(const MainMenuDTO& main_menu);

    bool serialize_and_send_action(const ActionDTO& action);

    std::string receive_and_deserialize_games_names();
};

#endif  // CLIENT_PROTOCOL_H
