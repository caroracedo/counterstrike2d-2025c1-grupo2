#ifndef CLIENT_H
#define CLIENT_H

#include <utility>

#include "client_protocol.h"
#include "mock_handler.h"

class Client {
private:
    Socket client_socket;
    ClientProtocol protocol;
    MockHandler mock_handler;

public:
    explicit Client(const char* hostname, const char* servname):
            client_socket(hostname, servname), protocol(this->client_socket) {}

    void initiate_communication() {
        try {
            while (true) {
                if (!protocol.serialize_and_send_action(mock_handler.receive_and_parse_action()))
                    break;

                ActionDTO action_update = protocol.receive_and_deserialize_updated_position();

                if (!mock_handler.send_action(action_update))
                    break;
            }
        } catch (...) {}  // Por el momento...

        client_socket.shutdown(2);  // Cierra lectura y escritura
        client_socket.close();
    }
};

#endif  // CLIENT_H
