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

    bool serialize_and_send_move(ActionDTO action_dto) {
        std::vector<uint8_t> data;
        data.push_back(1 + action_dto.position.size());
        data.push_back(MOVE_OPCODE);
        data.insert(data.end(), action_dto.position.begin(), action_dto.position.end());
        return skt_manager.send_bytes(skt, data);
    }

    void close();
};

#endif  // SERVER_PROTOCOL_H
