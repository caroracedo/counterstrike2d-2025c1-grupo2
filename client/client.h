#ifndef CLIENT_H
#define CLIENT_H

#include "client_receiver.h"
#include "client_sender.h"
#include "mock_handler.h"

class Client {
private:
    Socket client_socket;
    ClientProtocol protocol;
    MockHandler mock_handler;

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

        while (true) {
            try {
                to_server.push(mock_handler.receive_and_parse_action());

                ActionDTO action_update;
                if (from_server.try_pop(action_update) && !mock_handler.send_action(action_update))
                    break;
            } catch (...) {
                break;
            }
        }

        sender.stop();
        receiver.stop();
        sender.join();
        receiver.join();

        client_socket.shutdown(2);  // Cierra lectura y escritura
        client_socket.close();
    }
};

#endif  // CLIENT_H
