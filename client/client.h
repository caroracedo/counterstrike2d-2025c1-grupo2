#ifndef CLIENT_H
#define CLIENT_H

#include "common/action_DTO.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"

class Client {
private:
    /* Configuración */
    Socket client_socket;
    ClientProtocol protocol;

    Queue<ActionDTO> send_queue;
    Queue<ActionDTO> recv_queue;

    ClientSender sender;
    ClientReceiver receiver;

    /* Ejecución */
    ActionDTO lobby();
    void match_loop(const ActionDTO& configuration);

public:
    /* Constructor */
    Client(const char* hostname, const char* servname);

    /* Ejecución */
    void run();
};

#endif  // CLIENT_H
