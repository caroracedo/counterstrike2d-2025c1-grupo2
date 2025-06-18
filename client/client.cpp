#include "client.h"

#include <QApplication>
#include <vector>

#include "graphics/game_view.h"
#include "graphics/input_handler.h"
#include "lobby/mainwindow.h"

/* Constructor */
Client::Client(const char* hostname, const char* servname):
        client_socket(hostname, servname),
        protocol(this->client_socket),
        sender(protocol, send_queue),
        receiver(protocol, recv_queue) {}

/* Ejecución */
ActionDTO Client::lobby() {
    ActionDTO information = protocol.receive_and_deserialize_action();

    int argc = 0;
    char** argv = nullptr;
    QApplication app(argc, argv);
    MainWindow window(information.maps, information.matches);
    window.show();
    app.exec();

    protocol.serialize_and_send_action(window.getInfo());

    return protocol.receive_and_deserialize_action();
}

void Client::run() {
    ActionDTO configuration = lobby();

    sender.start();
    receiver.start();

    GameView game_view;
    game_view.set_id(configuration.id);
    game_view.set_terrain(configuration.terrain_type);
    game_view.pre_lobby(true);

    InputHandler input_handler(game_view);

    bool stop_flag = false;
    while (!stop_flag) {
        std::vector<ActionDTO> actions = input_handler.receive_and_parse_actions();
        for (const auto& action: actions) {
            if (action.type != ActionType::UNKNOWN) {
                send_queue.push(action);
                if (action.type == ActionType::QUIT) {
                    stop_flag = true;
                    break;
                }
            }
        }

        ActionDTO update;
        while (recv_queue.try_pop(update)) {
            game_view.pre_lobby(false);
            if (update.type == ActionType::END || update.type == ActionType::UNKNOWN) {
                stop_flag = true;
                break;
            } else if (update.type == ActionType::UPDATE || update.type == ActionType::SHOP ||
                       update.type == ActionType::STATS) {
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
