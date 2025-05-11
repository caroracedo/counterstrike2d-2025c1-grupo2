#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>
#include <vector>

#include "../common/action_DTO.h"
#include "../common/constants.h"
#include "../common/main_menu_DTO.h"
#include "../common/socket.h"
#include "../common/socket_manager.h"

/*
Clase que encapsula el protocolo de comunicación del Server con el Client.
Se encarga del envío y recepción de mensajes.
*/
class ServerProtocol {
private:
    Socket skt;
    SocketManager skt_manager;

    MainMenuDTO receive_and_deserialize_game_name(const Option& action_type);
    ActionDTO receive_and_deserialize_move();

public:
    explicit ServerProtocol(Socket&& skt);

    MainMenuDTO receive_and_deserialize_main_menu_action();

    bool serialize_and_send_games_names(const std::vector<std::string>& names);

    ActionDTO receive_and_deserialize_action();

    bool serialize_and_send_updated_position(ActionDTO action_dto);

    void close();
};

#endif  // SERVER_PROTOCOL_H
