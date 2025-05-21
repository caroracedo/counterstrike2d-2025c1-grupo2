#ifndef CLIENT_H
#define CLIENT_H

#include <utility>

#include "client_receiver.h"
#include "client_sender.h"
#include "client_protocol.h"
#include "../common/action_DTO.h"
#include "input_handler.h"
#include "game_view.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Surface.hh>
#include <cmath>


using namespace SDL2pp;

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
        
        GameView gv; //peldon por los nombres 
        InputHandler ih;

        // LoopClient
        while (true) {
            try {
                ActionDTO action = ih.receive_and_parse_action();
                if (action.type == ActionType::QUIT)
                    break;
                if (action.type == ActionType::UNKNOWN) {
                    gv.render();
                    continue;
                }
                to_server.push(action);
                
                ActionDTO action_update = from_server.pop();
                
                if (action_update.type == ActionType::UNKNOWN) {
                    continue;
                }  

                gv.update(action_update);
                gv.render();

            } catch (...) {}  // Por el momento...
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
