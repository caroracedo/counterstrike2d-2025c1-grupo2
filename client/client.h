#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <utility>

#include "../common/action_DTO.h"
#include "../common/types.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"
#include "game_view.h"
#include "input_handler.h"
#include "mock_handler.h"

class Client {
private:
    Socket client_socket;
    ClientProtocol protocol;

    Queue<ActionDTO> send_queue;
    Queue<ActionDTO> recv_queue;

    Sender sender;
    Receiver receiver;

    ActionType operation_type;
    std::string match;
    PlayerType player_type;

    ActionType parse_operation(const char* operation_type_str) {
        std::string operation(operation_type_str);
        if (operation == "Create")
            return ActionType::CREATE;
        return ActionType::JOIN;
    }

    PlayerType parse_player_type(const char* type_str) {
        std::string type(type_str);
        if (type == "Terrorist")
            return PlayerType::TERRORIST;
        return PlayerType::COUNTERTERRORIST;
    }

    void send_initial_configuration() { send_queue.push({operation_type, match, player_type}); }

public:
    Client(const char* hostname, const char* servname, const char* operation_type,
           const char* match, const char* type_str):
            client_socket(hostname, servname),
            protocol(this->client_socket),
            sender(protocol, send_queue),
            receiver(protocol, recv_queue),
            operation_type(parse_operation(operation_type)),
            match(match),
            player_type(parse_player_type(type_str)) {}

    void run() {
        sender.start();
        receiver.start();

        send_initial_configuration();

        GameView game_view;
        InputHandler input_handler(game_view.get_camera());

        bool stop_flag = false;
        while (!stop_flag) {
            ActionDTO action = input_handler.receive_and_parse_action();
            if (action.type == ActionType::QUIT) {
                break;
            } else if (action.type != ActionType::UNKNOWN) {
                send_queue.push(action);
            }

            ActionDTO update;
            while (recv_queue.try_pop(update)) {
                if (update.type == ActionType::END || update.type == ActionType::UNKNOWN) {
                    stop_flag = true;
                    break;
                } else if (update.type == ActionType::PLAYERID) {
                    game_view.set_id(update.id);
                } else if (update.type == ActionType::UPDATE) {
                    game_view.update(update);
                }
            }

            game_view.render();
            game_view.frame_sync();
        }

        client_socket.shutdown(2);
        client_socket.close();

        sender.stop();
        receiver.stop();
        sender.join();
        receiver.join();
    }
};

#endif  // CLIENT_H
