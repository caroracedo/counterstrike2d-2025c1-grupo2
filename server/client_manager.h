#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <string>

#include "../common_src/monitor_games.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"

#include "server_protocol.h"

class ClientManager: public Thread {
private:
    ServerProtocol protocol;
    MonitorGames& monitor_games;
    std::string game_name;
    Turn turn;

    bool do_action(const ActionDTO& action_dto) {
        // Y move?
        switch (action_dto.type) {
            case ActionType::CREATE:
                return do_create_action(action_dto);
            case ActionType::LIST:
                return do_list_action(action_dto);
            case ActionType::JOIN:
                return do_join_action(action_dto);
            // case ActionType::PLAY:
            //     return do_play_action(action_dto);
            default:
                return false;
        }
    }

    bool do_create_action(const ActionDTO& action_dto) {
        MonitorGame* monitor_game = monitor_games.create_game(action_dto.game_name);
        if (!monitor_game)
            return false;
        game_name = action_dto.game_name;
        turn = Turn::PLAYER1;
        return protocol.serialize_and_send_action(
                {GameType::GAME, monitor_game->get_formatted_board()});
    }

    bool do_list_action(const ActionDTO& action_dto) {
        return protocol.serialize_and_send_action({GameType::GAME, monitor_games.list_games()});
    }

    bool do_join_action(const ActionDTO& action_dto) {
        MonitorGame* monitor_game = monitor_games.get_game(action_dto.game_name);
        if (!monitor_game)
            return false;
        game_name = action_dto.game_name;
        turn = Turn::PLAYER2;
        if (!monitor_game->join_game(turn))
            return false;
        return protocol.serialize_and_send_action(
                {GameType::GAME, monitor_game->get_formatted_board()});
    }

    bool do_play_action(const ActionDTO& action_dto) {
        MonitorGame* monitor_game = monitor_games.get_game(game_name);
        if (!monitor_game)
            return false;
        if (!monitor_game->play_game(turn, action_dto.column, action_dto.row))
            return false;
        std::string board = monitor_game->get_formatted_board();
        // bool is_over = modify_if_its_over(monitor_game, board);
        return protocol.serialize_and_send_action({GameType::GAME, board}) && !is_over;
    }

public:
    /*
     * Constructor.
     **/
    ClientManager(Socket&& client_socket, MonitorGames& monitor_games);

    /* Override */
    void run() override {
        // Mientras juego no esté creado
        while (should_keep_running()) {
            try {
                ActionDTO action_dto = protocol.receive_and_deserialize_action();
                if (!do_action(action_dto))
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
