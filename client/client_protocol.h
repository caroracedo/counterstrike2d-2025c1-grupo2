#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstring>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "../common/action_DTO.h"
#include "../common/constants.h"
#include "../common/main_menu_DTO.h"
#include "../common/socket.h"
#include "../common/socket_manager.h"

class ClientProtocol {
private:
    Socket& skt;
    SocketManager skt_manager;

    int hex_big_endian_to_int(const std::vector<uint8_t>& hex) {
        uint16_t integer;
        std::memcpy(&integer, hex.data(), sizeof(integer));
        integer = ntohs(integer);
        return static_cast<int>(integer);
    }

public:
    explicit ClientProtocol(Socket& skt);

    bool serialize_and_send_action(const ActionDTO& action);

    ActionDTO receive_and_deserialize_updated_position();
};

#endif  // CLIENT_PROTOCOL_H
