#ifndef CLIENT_H
#define CLIENT_H

#include <QApplication>
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
#include "mainwindow.h"
#include "mock_handler.h"

class Client {
private:
    Socket client_socket;
    ClientProtocol protocol;

    Queue<ActionDTO> send_queue;
    Queue<ActionDTO> recv_queue;

    Sender sender;
    Receiver receiver;

    void send_initial_configuration() {
        int argc = 0;
        char** argv = nullptr;
        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        app.exec();
        send_queue.push(window.getInfo());
    }

public:
    Client(const char* hostname, const char* servname):
            client_socket(hostname, servname),
            protocol(this->client_socket),
            sender(protocol, send_queue),
            receiver(protocol, recv_queue) {}

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
                } else if (update.type == ActionType::UPDATE || update.type == ActionType::SHOP) {
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
