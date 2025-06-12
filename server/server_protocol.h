#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstring>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "../common/action_DTO.h"
#include "../common/socket.h"
#include "../common/socket_manager.h"

class ServerProtocol {
private:
    Socket& skt;
    SocketManager skt_manager;
    uint16_t id;

    // TODO: Abstraer esto...
    uint16_t hex_big_endian_to_int_16(const std::vector<uint8_t>& hex_big_endian) {
        uint16_t int_16;
        std::memcpy(&int_16, hex_big_endian.data(), sizeof(int_16));
        return ntohs(int_16);
    }

    std::vector<uint8_t> int_16_to_hex_big_endian(const uint16_t int_16) {
        uint16_t htons_int_16 = htons(int_16);
        std::vector<uint8_t> hex_big_endian(sizeof(htons_int_16));
        std::memcpy(hex_big_endian.data(), &htons_int_16, sizeof(htons_int_16));
        return hex_big_endian;
    }

    void push_hexa_to(const std::vector<uint8_t>& hexa, std::vector<uint8_t>& vector) {
        vector.push_back(hexa[0]);
        vector.push_back(hexa[1]);
    }

    /* Envío */
    void serialize_and_send_update(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_configuration(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_shop(const ActionDTO& action_dto, std::vector<uint8_t>& data);
    void serialize_and_send_stats(const ActionDTO& action_dto, std::vector<uint8_t>& data);

public:
    /* Constructor */
    ServerProtocol(Socket& skt, uint16_t id);

    /* Recepción */
    ActionDTO receive_and_deserialize_action();

    /* Envío */
    bool serialize_and_send_action(const ActionDTO& action_dto);
};

#endif  // SERVER_PROTOCOL_H
