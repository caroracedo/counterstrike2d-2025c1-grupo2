#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>
#include <vector>

#include "../common/action_DTO.h"
#include "../common/constants.h"
#include "../common/main_menu_DTO.h"
#include "../common/socket.h"
#include "../common/socket_manager.h"

class ClientProtocol {
private:
    Socket& skt;
    SocketManager skt_manager;

public:
    explicit ClientProtocol(Socket& skt);

    bool serialize_and_send_action(const ActionDTO& action);

    ActionDTO receive_and_deserialize_updated_position();
};

#endif  // CLIENT_PROTOCOL_H
