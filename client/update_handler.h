#ifndef UPDATE_HANDLER_H
#define UPDATE_HANDLER_H

#include <atomic>

#include "../common/queue.h"
#include "../common/thread.h"

#include "game_view.h"

class UpdateHandler: public Thread {
private:
    Queue<ActionDTO>& recv_queue;
    std::atomic<bool>& stop_flag;
    GameView& game_view;

public:
    UpdateHandler(Queue<ActionDTO>& recv_queue, std::atomic<bool>& stop_flag, GameView& game_view):
            recv_queue(recv_queue), stop_flag(stop_flag), game_view(game_view) {}

    void run() override {
        while (should_this_thread_keep_running()) {
            try {
                ActionDTO action_update;
                while (!recv_queue.try_pop(action_update)) {}
                if (action_update.type == ActionType::UNKNOWN) {
                    std::cout << "Acción desconocida recibida. Terminando el hilo de actualización." << std::endl;
                    break;
                } else if (action_update.type == ActionType::PLAYERID) {
                    std::cout << "ID recibido: " << action_update.id << std::endl;
                    game_view.set_id(action_update.id);
                } else if (action_update.type == ActionType::UPDATE) {
                    game_view.update(action_update);
                }
            } catch (...) {
                break;
            }
        }
        stop_flag = true;
    }

    bool should_this_thread_keep_running() const { return should_keep_running() && !stop_flag; }
};

#endif  // UPDATE_HANDLER_H
