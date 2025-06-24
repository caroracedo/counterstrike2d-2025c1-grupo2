#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <cstdint>
#include <string>
#include <vector>

#include "socket.h"

class SocketManager {
public:
    /* Recepción */
    bool receive_byte(Socket& socket, uint8_t& byte);
    bool receive_two_bytes(Socket& socket, uint16_t& bytes);
    bool receive_bytes(Socket& socket, std::vector<uint8_t>& bytes);

    /* Envío */
    bool send_byte(Socket& socket, const uint8_t& byte);
    bool send_two_bytes(Socket& socket, const uint16_t& bytes);
    bool send_bytes(Socket& socket, const std::vector<uint8_t>& bytes);

    /* Cierre */
    void kill(Socket& socket);
};

#endif  // SOCKET_MANAGER_H
