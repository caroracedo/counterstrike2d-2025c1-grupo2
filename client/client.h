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
                ActionDTO action = mock_handler.receive_and_parse_action();
                if (action.type == ActionType::UNKNOWN)
                    break;
                to_server.push(action);

                if (!mock_handler.send_action(
                            from_server.pop()))  // TODO: Preguntar si se puede hacer con pop
                    break;
            } catch (...) {
                break;
            }
        }
        // TODO: Preguntar si está bien cerrar el socket acá y por qué en el servidor funciona...
        client_socket.shutdown(2);  // Cierra lectura y escritura
        client_socket.close();

        sender.stop();
        receiver.stop();
        sender.join();
        receiver.join();
    }
};

#endif  // CLIENT_H
