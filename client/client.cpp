#include "client.h"

#include <QApplication>
#include <vector>

#include "graphics/game_view.h"
#include "graphics/input_handler.h"
#include "lobby/mainwindow.h"

Client::Client(const char* hostname, const char* servname):
        client_socket(hostname, servname),
        protocol(this->client_socket),
        sender(protocol, send_queue),
        receiver(protocol, recv_queue) {}

void Client::receive_and_send_initial_configuration() {  // TODO: Esto esta un poco feo...
    ActionDTO configuration =
            recv_queue.pop();  // TODO: Ver si esto está bien o mal... porque técnicamente se
                               // debería bloquear hasta recibir la configuración

    int argc = 0;
    char** argv = nullptr;
    QApplication app(argc, argv);
    MainWindow window(configuration.maps, configuration.matches);
    window.show();
    app.exec();

    send_queue.push(window.getInfo());
}

void Client::run() {
    sender.start();
    receiver.start();

    receive_and_send_initial_configuration();

    GameView game_view;
    InputHandler input_handler(game_view);

    bool stop_flag = false;
    while (!stop_flag) {
        std::vector<ActionDTO> actions = input_handler.receive_and_parse_actions();
        for (const auto& action: actions) {
            if (action.type == ActionType::QUIT) {
                stop_flag = true;
                break;
            } else if (action.type != ActionType::UNKNOWN) {
                send_queue.push(action);
            }
        }

        ActionDTO update;
        while (recv_queue.try_pop(update)) {
            game_view.pre_lobby(false);
            if (update.type == ActionType::END || update.type == ActionType::UNKNOWN) {
                stop_flag = true;
                break;
            } else if (update.type == ActionType::CONFIGURATION) {
                game_view.set_id(update.id);
                game_view.set_terrain(update.terrain_type);
            } else if (update.type == ActionType::UPDATE || update.type == ActionType::SHOP ||
                       update.type == ActionType::STATS) {
                game_view.update(update);
            } else if (update.type == ActionType::WAIT) {
                game_view.pre_lobby(true);
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
