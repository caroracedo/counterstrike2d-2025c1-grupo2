#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>

#include "../common/action_DTO.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"
#include "mock_handler.h"

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

        MockHandler mock_handler;

        // LoopClient
        while (true) {
            try {
                ActionDTO action = mock_handler.receive_and_parse_action();
                if (action.type == ActionType::QUIT || action.type == ActionType::UNKNOWN)
                    break;  // Error o salir
                to_server.push(action);


                ActionDTO action_update;
                while (from_server.try_pop(action_update)) {}  // Había dicho un while...
                if (action_update.type == ActionType::UNKNOWN ||
                    !mock_handler.update_graphics(action_update))
                    break;  // Error
            } catch (...) {
                break;
            }
        }

        // TODO: Preguntar si está bien cerrar el socket acá y por qué en el servidor funciona...
        // Ahora sospecho que es por el try_pop
        client_socket.shutdown(2);  // Cierra lectura y escritura
        client_socket.close();

        sender.stop();
        receiver.stop();
        sender.join();
        receiver.join();
    }
};

#endif  // CLIENT_H
