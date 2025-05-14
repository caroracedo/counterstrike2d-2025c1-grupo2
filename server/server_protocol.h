#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <iostream>
#include <string>
#include <vector>

#include "../common/action_DTO.h"
#include "../common/constants.h"
#include "../common/main_menu_DTO.h"
#include "../common/socket.h"
#include "../common/socket_manager.h"

class ServerProtocol {
private:
    Socket& skt;
    SocketManager skt_manager;

public:
    explicit ServerProtocol(Socket& skt);

    ActionDTO receive_and_deserialize_action();

    bool serialize_and_send_updated_position(ActionDTO action_dto);
};

#endif  // SERVER_PROTOCOL_H
