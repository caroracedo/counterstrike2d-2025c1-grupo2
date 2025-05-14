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
    Socket client_socket;
    ServerProtocol protocol;
    MonitorGame& monitor_game;

    bool do_action(const ActionDTO& action_dto) {
        switch (action_dto.type) {
            case ActionType::MOVE:
                return do_move_action(action_dto);
            default:
                return false;
        }
    }

    bool do_move_action(const ActionDTO& action_dto) {
        if (!monitor_game.move(action_dto.direction))
            return false;
        return protocol.serialize_and_send_updated_position(
                {ActionType::MOVE, monitor_game.get_position()});
    }

    void run_loop(std::function<bool()> action_loop) {
        while (should_keep_running()) {
            try {
                if (!action_loop()) {
                    return;
                }
            } catch (...) {
                return;
            }
        }
    }

public:
    /*
     * Constructor.
     **/
    ClientHandler(Socket&& client_socket, MonitorGame& monitor_game):
            client_socket(std::move(client_socket)),
            protocol(this->client_socket),
            monitor_game(monitor_game) {}

    /* Override */
    void run() override {
        run_loop([&]() { return do_action(protocol.receive_and_deserialize_action()); });
    }

    /* Matar Hilo */
    void hard_kill() {
        Thread::stop();
        client_socket.shutdown(2);  // Cierra lectura y escritura
        client_socket.close();
    }
};

#endif  // CLIENT_HANDLER_H
