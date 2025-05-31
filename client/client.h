#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>

#include "../common/action_DTO.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"
#include "event_handler.h"
#include "game_view.h"
#include "input_handler.h"
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

    // void initiate_communication() {
    //     sender.start();
    //     receiver.start();

    //     // TODO: Capaz conviene no tenerlos como atributo, si solo se usan en esta función...
    //     GameView game_view;
    //     InputHandler input_handler;

    //     while (true) {
    //         try {
    //             ActionDTO action = input_handler.receive_and_parse_action();
    //             if (action.type == ActionType::QUIT)
    //                 break;
    //             if (action.type == ActionType::UNKNOWN) {
    //                 game_view.render();
    //                 continue;
    //             }
    //             to_server.push(action);
                
    //             ActionDTO action_update;
    //             if (from_server.try_pop(action_update)) {
    //                 if (action_update.type == ActionType::UNKNOWN) {
    //                     continue;
    //                 }               
    //                 game_view.update(action_update);
    //             }
    //             game_view.render();
    //         } catch (...) {}
    //     }


    //     // Cuando termina uno debería terminar todo...
    //     // event_handler.start();
    //     // update_handler.start();

    //     // event_handler.join();
    //     // update_handler.join();
        
    //     client_socket.shutdown(2);
    //     client_socket.close();

    //     sender.stop();
    //     receiver.stop();
    //     sender.join();
    //     receiver.join();
    // }
    void initiate_communication() {
        sender.start();
        receiver.start();

        InputHandler input_handler;
        GameView game_view;
        std::atomic<bool> stop_flag = false;
        EventHandler event_handler(to_server, stop_flag, input_handler);
        UpdateHandler update_handler(from_server, stop_flag, game_view);

        event_handler.start();
        update_handler.start();

         while (!stop_flag) {
            game_view.render();
            game_view.frame_sync();
        }

        event_handler.join();
        update_handler.join();
        // bool running = true;
        // Uint32 last_frame_time = SDL_GetTicks();
        // const Uint32 frame_delay = 1000 / 60;  // 60 FPS

        // while (running) {
        //     // INPUT
        //     ActionDTO action = input_handler.receive_and_parse_action();
        //     if (action.type == ActionType::QUIT) {
        //         running = false;
        //     } else if (action.type != ActionType::UNKNOWN) {
        //         to_server.push(action);
        //     }

        //     // SERVER UPDATE
        //     ActionDTO update;
        //     while (from_server.try_pop(update)) {
        //         if (update.type == ActionType::UPDATE) {
        //             game_view.update(update);
        //         }
        //     }

        //         // RENDER
        //     game_view.render();
        //     game_view.frame_sync();

        // }

        client_socket.shutdown(2);
        client_socket.close();

        sender.stop();
        receiver.stop();
        sender.join();
        receiver.join();
    }

};

#endif  // CLIENT_H
