#include "client.h"

#include <QApplication>
#include <vector>

#include "lobby/mainwindow.h"
#include "prelobby/prelobby.h"

/* Ejecución */
ActionDTO Client::prelobby() {
    int argc = 0;
    char** argv = nullptr;
    QApplication app(argc, argv);
    PreLobby window;
    window.show();
    app.exec();

    return window.getInfo();
}

ActionDTO Client::lobby(ClientProtocol& protocol) {
    ActionDTO information = protocol.receive_and_deserialize_action();

    if (information.type != ActionType::INFORMATION)
        return ActionDTO();

    int argc = 0;
    char** argv = nullptr;
    QApplication app(argc, argv);
    MainWindow window(information.maps, information.matches);
    window.show();
    app.exec();

    if (!protocol.serialize_and_send_action(window.getInfo()))
        return ActionDTO();

    return protocol.receive_and_deserialize_action();
}

void Client::match_loop(InputHandler& input_handler, GameView& game_view) {
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
}

void Client::run() {
    ActionDTO init = prelobby();

    if (init.type != ActionType::INIT)
        return;

    Socket client_socket(init.hostname.c_str(), init.servname.c_str());
    ClientProtocol protocol(client_socket);

    ActionDTO configuration = lobby(protocol);

    if (configuration.type != ActionType::CONFIGURATION)
        return;

    ClientSender sender(protocol, send_queue);
    ClientReceiver receiver(protocol, recv_queue);

    GameView game_view;
    game_view.set_id(configuration.id);
    game_view.set_terrain(configuration.terrain_type);
    game_view.pre_lobby(true);
    InputHandler input_handler(game_view);

    sender.start();
    receiver.start();

    match_loop(input_handler, game_view);

    client_socket.shutdown(2);
    client_socket.close();

    sender.stop();
    receiver.stop();
    sender.join();
    receiver.join();
}
