#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include "../common/socket.h"
#include "../common/socket_manager.h"
#include "../common/action_DTO.h"

/*
Clase que encapsula el protocolo de comunicación del Server con el Client.
Se encarga del envío y recepción de mensajes.
*/
class ServerProtocol {
private:
    Socket skt;
    SocketManager skt_manager;

    ActionDTO receive_and_deserialize_game(const ActionType& action_type);
    ActionDTO receive_and_deserialize_list();

public:
    explicit ServerProtocol(Socket&& skt);

    ActionDTO receive_and_deserialize_action();

    void close();
};

#endif // SERVER_PROTOCOL_H