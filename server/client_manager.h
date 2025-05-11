#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <string>
#include <utility>

#include "../common/monitor_games.h"
#include "../common/socket.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ClientManager: public Thread {
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
        // Acá debería enviar el estado del juego? Debería devolver booleano si salió todo bien o
        // no? return protocol.serialize_and_send_games_names({});
        return true;
    }

    bool do_list_option(const MainMenuDTO& main_menu_dto) {
        // Acá debería recibir un MainMenuDTO Debería devolver booleano si salió todo bien o no?
        // return protocol.serialize_and_send_games_names({Option::LIST,
        // monitor_games.list_games()});
        return true;
    }

    bool do_join_option(const MainMenuDTO& main_menu_dto) {
        if (!monitor_games.join_game(main_menu_dto.game_name))
            return false;
        game_name = main_menu_dto.game_name;
        // Acá debería enviar el estado del juego? Debería devolver booleano si salió todo bien o
        // no? return protocol.serialize_and_send_games_names({});
        return true;
    }

    bool do_move_action(const ActionDTO& action_dto) {
        if (!monitor_games.move_game(game_name, action_dto.direction))
            return false;
        // Acá debería enviar el estado del juego? Debería devolver booleano si salió todo bien o
        // no? return protocol.serialize_and_send_action({});

        return true;
    }

public:
    /*
     * Constructor.
     **/
    ClientManager(Socket&& client_socket, MonitorGames& monitor_games):
            protocol(std::move(client_socket)), monitor_games(monitor_games) {}

    /* Override */
    void run() override {
        // Mientras juego no esté creado
        while (should_keep_running()) {
            try {
                MainMenuDTO main_menu_dto = protocol.receive_and_deserialize_main_menu_action();
                if (!do_main_menu_option(main_menu_dto))
                    break;
            } catch (...) {
                break;
            }
        }
        // Cuando juego está creado
        while (should_keep_running()) {
            try {
                ActionDTO action_dto = protocol.receive_and_deserialize_action();
                if (!do_action(action_dto))
                    break;
            } catch (...) {
                break;
            }
        }
        protocol.close();
    }
};

#endif  // CLIENT_MANAGER_H
