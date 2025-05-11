#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <functional>
#include <string>
#include <utility>

#include "../common/monitor_games.h"
#include "../common/socket.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ClientHandler: public Thread {
private:
    ServerProtocol protocol;
    MonitorGames& monitor_games;
    std::string game_name;

    bool do_action(const ActionDTO& action_dto) {
        switch (action_dto.type) {
            case ActionType::MOVE:
                return do_move_action(action_dto);
            default:
                return false;
        }

        /*
        Esta función debería encolar el evento que se quiere realizar a la queue de GameMonitor
        GameMonitor se tendría que encargar de desencolar eventos y updatear el mapa según
        corresponda. La acción de encolar podría devolver el iterador, que a su vez se debería
        enviar al cliente para que actualice la imagen segun corresponda
        */
    }

    bool do_main_menu_option(const MainMenuDTO& main_menu_dto) {
        switch (main_menu_dto.option) {
            case Option::CREATE:
                return do_create_option(main_menu_dto);
            case Option::LIST:
                return do_list_option(main_menu_dto);
            case Option::JOIN:
                return do_join_option(main_menu_dto);
            default:
                return false;
        }
    }

    bool do_create_option(const MainMenuDTO& main_menu_dto) {
        if (!monitor_games.create_game(main_menu_dto.game_name))
            return false;
        game_name = main_menu_dto.game_name;
        // Acá debería enviar el estado del juego
        // return protocol.serialize_and_send_games_names({});
        return true;
    }

    bool do_list_option(const MainMenuDTO& main_menu_dto) {
        // return protocol.serialize_and_send_games_names({Option::LIST,
        // monitor_games.list_games()});
        return true;
    }

    bool do_join_option(const MainMenuDTO& main_menu_dto) {
        if (!monitor_games.join_game(main_menu_dto.game_name))
            return false;
        game_name = main_menu_dto.game_name;
        // Acá debería enviar el estado del juego
        // return protocol.serialize_and_send_games_names({});
        return true;
    }

    bool do_move_action(const ActionDTO& action_dto) {
        if (!monitor_games.move_game(game_name, action_dto.direction))
            return false;
        return protocol.serialize_and_send_move(
                {ActionType::MOVE, monitor_games.get_position_game(game_name)});
    }

    void run_loop(std::function<bool()> action_loop) {
        while (should_keep_running()) {
            try {
                if (!action_loop()) {
                    protocol.close();
                    return;
                }
            } catch (...) {
                protocol.close();
                return;
            }
        }
    }

public:
    /*
     * Constructor.
     **/
    ClientHandler(Socket&& client_socket, MonitorGames& monitor_games):
            protocol(std::move(client_socket)), monitor_games(monitor_games) {}

    /* Override */
    void run() override {
        // Enfoque funcional, sacando código repetido
        // Descomento para que me deje commitear
        run_loop([&]() {
            MainMenuDTO dto = protocol.receive_and_deserialize_main_menu_action();
            if (!do_main_menu_option(dto))
                return false;
            if (dto.option == Option::CREATE || dto.option == Option::JOIN)
                return false;
            return true;
        });

        run_loop([&]() {
            ActionDTO dto = protocol.receive_and_deserialize_action();
            return do_action(dto);
        });

        // Thread::stop;
        protocol.close();
    }

    // Para el clear
    // void hard_kill() {
    //     Thread::stop;
    //     protocol.close();
    // }
};

#endif  // CLIENT_HANDLER_H
