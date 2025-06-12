#ifndef CLIENT_H
#define CLIENT_H

#include "../common/action_DTO.h"
#include "../common/types.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"

class Client {
private:
    Socket client_socket;
    ClientProtocol protocol;

    Queue<ActionDTO> send_queue;
    Queue<ActionDTO> recv_queue;

    ClientSender sender;
    ClientReceiver receiver;

    uint16_t receive_and_send_initial_configuration();

public:
    Client(const char* hostname, const char* servname);
    void run();
};

#endif  // CLIENT_H
