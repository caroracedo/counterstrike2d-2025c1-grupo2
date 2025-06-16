#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "../common/action_DTO.h"
#include "../common/socket.h"
#include "../common/socket_manager.h"

class ClientProtocol {
private:
    Socket& skt;
    SocketManager skt_manager;

    // TODO: Abstraer esto...
    uint16_t hex_big_endian_to_int_16(const std::vector<uint8_t>& hex_big_endian) {
        uint16_t int_16;
        std::memcpy(&int_16, hex_big_endian.data(), sizeof(int_16));
        return ntohs(int_16);
    }

    float hex_big_endian_to_float(const std::vector<uint8_t>& hex_big_endian) {
        uint32_t int_32;
        std::memcpy(&int_32, hex_big_endian.data(), sizeof(int_32));
        int_32 = ntohl(int_32);
        float valor;
        std::memcpy(&valor, &int_32, sizeof(valor));
        return valor;
    }

    std::vector<uint8_t> float_to_hex_big_endian(float valor) {
        uint32_t raw;
        std::memcpy(&raw, &valor, sizeof(raw));
        raw = htonl(raw);
        std::vector<uint8_t> hex_big_endian(sizeof(raw));
        std::memcpy(hex_big_endian.data(), &raw, sizeof(raw));
        return hex_big_endian;
    }

    std::vector<uint8_t> int_16_to_hex_big_endian(const uint16_t int_16) {
        uint16_t htons_int_16 = htons(int_16);
        std::vector<uint8_t> hex_big_endian(sizeof(htons_int_16));
        std::memcpy(hex_big_endian.data(), &htons_int_16, sizeof(htons_int_16));
        return hex_big_endian;
    }

    void push_hexa_to(const std::vector<uint8_t>& hexa, std::vector<uint8_t>& vector) {
        vector.insert(vector.end(), hexa.begin(), hexa.end());
    }

    /* Recepción */
    ActionDTO deserialize_update(std::vector<uint8_t>& data);
    ActionDTO deserialize_information(std::vector<uint8_t>& data);
    ActionDTO deserialize_configuration(std::vector<uint8_t>& data);
    ActionDTO deserialize_end();
    ActionDTO deserialize_shop(std::vector<uint8_t>& data);
    ActionDTO deserialize_stats(std::vector<uint8_t>& data);

public:
    /* Constructor */
    explicit ClientProtocol(Socket& skt);

    /* Recepción */
    ActionDTO receive_and_deserialize_action();

    /* Envío */
    bool serialize_and_send_action(const ActionDTO& action);
};

#endif  // CLIENT_PROTOCOL_H
