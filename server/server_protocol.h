#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "../common/action_DTO.h"
#include "../common/constants.h"
#include "../common/main_menu_DTO.h"
#include "../common/socket.h"
#include "../common/socket_manager.h"

class ServerProtocol {
private:
    Socket& skt;
    SocketManager skt_manager;
    int id;

    std::vector<uint8_t> int_to_hex_big_endian(const int& integer) {
        uint16_t integer_16 = htons(static_cast<uint16_t>(integer));
        std::vector<uint8_t> hex(sizeof(integer_16));
        std::memcpy(hex.data(), &integer_16, sizeof(integer_16));
        return hex;
    }

    void push_hexa_to(const std::vector<uint8_t>& hexa, std::vector<uint8_t>& vector) {
        vector.push_back(hexa[0]);
        vector.push_back(hexa[1]);
    }

public:
    ServerProtocol(Socket& skt, int id);

    ActionDTO receive_and_deserialize_action();

    bool serialize_and_send_updated_game(const ActionDTO& action_dto);
};

#endif  // SERVER_PROTOCOL_H
