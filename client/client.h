#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>

#include "../common/action_DTO.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"
#include "event_handler.h"
#include "mock_handler.h"
#include "update_handler.h"

class Client {
private:
    Socket client_socket;
    ClientProtocol protocol;

    Queue<ActionDTO> to_server;
    Queue<ActionDTO> from_server;

    Sender sender;
    Receiver receiver;

public:
    Client(const char* hostname, const char* servname):
            client_socket(hostname, servname),
            protocol(this->client_socket),
            sender(protocol, to_server),
            receiver(protocol, from_server) {}

    void initiate_communication() {
        sender.start();
        receiver.start();

        // TODO: Capaz conviene no tenerlos como atributo, si solo se usan en esta función...
        MockHandler mock_handler;
        std::atomic<bool> stop_flag = false;
        EventHandler event_handler(to_server, mock_handler, stop_flag);
        UpdateHandler update_handler(from_server, mock_handler, stop_flag);

        // Cuando termina uno debería terminar todo...
        event_handler.start();
        update_handler.start();

        event_handler.join();
        update_handler.join();

        client_socket.shutdown(2);
        client_socket.close();

        sender.stop();
        receiver.stop();
        sender.join();
        receiver.join();
    }
};

#endif  // CLIENT_H
