#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <utility>
#include <vector>

// #include "client_graphic_parser.h"
#include "client_protocol.h"

class Client {
private:
    ClientProtocol protocol;
    // GraphicParser graphic_parser;

public:
    /*
     * Constructor.
     **/
    explicit Client(const char* hostname, const char* servname):
            protocol(std::move(Socket(hostname, servname))) {}

    /* Iniciar la comunicación */
    void initiate_communication() {
        while (true) {
            try {
                // Acá nos llega la acción

                // La parseamos

                // La mandamos con el protocolo

                // Recibo respuesta como DTO

                // Con eso hago X cosa
            } catch (...) {
                break;
            }
        }
        // protocol.close();
    }
};

#endif  // CLIENT_H
