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

    uint16_t hex_big_endian_to_int_16(const std::vector<uint8_t>& hex_big_endian) {
        uint16_t int_16;
        std::memcpy(&int_16, hex_big_endian.data(), sizeof(int_16));
        return ntohs(int_16);
    }

public:
    explicit ClientProtocol(Socket& skt);

    bool serialize_and_send_action(const ActionDTO& action);

    ActionDTO receive_and_deserialize_updated_position();
};

#endif  // CLIENT_PROTOCOL_H
