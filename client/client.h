#ifndef CLIENT_H
#define CLIENT_H

#include <utility>

#include "client_receiver.h"
#include "client_sender.h"
#include "client_protocol.h"
#include "../common/action_DTO.h"
#include "../common/action_DTO_prueba.h"
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
                
                if (ih.is_fire_requested()) 
                    gv.shoot(ih.get_fire_angle());

                if (action.type == ActionType::UNKNOWN) {
                    gv.update_bullets();
                    gv.render();
                    continue;
                }
                to_server.push(action);
                
                ActionDTO action_update;
                if (from_server.try_pop(action_update)) {
                    if (action_update.type != ActionType::UNKNOWN) {
                        ActionDTO2 action_update2;
                        ObjectDTO player;
                        player.type = ObjectType::PLAYER;
                        player.position = {action_update.position[0], action_update.position[1]};
                        player.width = 32;
                        player.height = 32;
                        player.id = 1;
                        action_update2.type = ActionType2::UPDATE;
                        action_update2.objects = std::vector<ObjectDTO>{player};
                        gv.update(action_update2);
                        gv.update_bullets();
                    }
                }

                
                if (action_update.type == ActionType::UNKNOWN) {
                    gv.update_bullets();
                    gv.render();
                    continue;
                }
                // ActionDTO2 action_update2;
                // ObjectDTO player;
                // player.type = ObjectType::PLAYER;
                // player.position = {action_update.position[0],action_update.position[1]};
                // player.width = 32;
                // player.height = 32;
                // player.id = 1;
                // action_update2.type = ActionType2::UPDATE;
                // action_update2.objects = std::vector<ObjectDTO>{player};
                // gv.update_bullets();
                // gv.update(action_update2);
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
