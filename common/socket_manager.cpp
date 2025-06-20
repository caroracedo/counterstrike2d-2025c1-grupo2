#include "socket_manager.h"

#include <arpa/inet.h>

/* Recibir */
bool SocketManager::receive_byte(Socket& socket, uint8_t& byte) {
    int sz = socket.recvall(&byte, sizeof(byte));
    if (sz != sizeof(byte) || socket.is_stream_recv_closed()) {
        return false;
    }
    return true;
}

bool SocketManager::receive_two_bytes(Socket& socket, uint16_t& bytes) {
    int sz = socket.recvall(&bytes, sizeof(bytes));
    if (sz != sizeof(bytes) || socket.is_stream_recv_closed()) {
        return false;
    }
    bytes = ntohs(bytes);
    return true;
}

bool SocketManager::receive_bytes(Socket& socket, std::vector<uint8_t>& bytes) {
    int sz = socket.recvall(bytes.data(), bytes.size());
    if (sz != static_cast<int>(bytes.size()) || socket.is_stream_recv_closed()) {
        return false;
    }
    return true;
}

/* Enviar */
bool SocketManager::send_byte(Socket& socket, const uint8_t& byte) {
    int sz = socket.sendall(&byte, sizeof(byte));
    if (sz != static_cast<int>(sizeof(byte)) || socket.is_stream_send_closed()) {
        return false;
    }
    return true;
}

bool SocketManager::send_two_bytes(Socket& socket, const uint16_t& bytes) {
    uint16_t bytes_to_send = htons(bytes);
    int sz = socket.sendall(&bytes_to_send, sizeof(bytes_to_send));
    if (sz != static_cast<int>(sizeof(bytes_to_send)) || socket.is_stream_send_closed()) {
        return false;
    }
    return true;
}

bool SocketManager::send_bytes(Socket& socket, const std::vector<uint8_t>& bytes) {
    int sz = socket.sendall(bytes.data(), bytes.size());
    if (sz != static_cast<int>(bytes.size()) || socket.is_stream_send_closed()) {
        return false;
    }
    return true;
}

/* Cerrar */
void SocketManager::kill(Socket& socket) {
    try {
        socket.shutdown(2);  // Cierra lectura y escritura
    } catch (...) {}
    socket.close();
}
