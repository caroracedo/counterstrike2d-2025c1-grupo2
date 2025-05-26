#ifndef UPDATE_HANDLER_H
#define UPDATE_HANDLER_H

#include <atomic>

#include "../common/queue.h"
#include "../common/thread.h"

#include "game_view.h"

class UpdateHandler: public Thread {
private:
    Queue<ActionDTO>& from_server;
    std::atomic<bool>& stop_flag;
    GameView& game_view;
    
public:
    UpdateHandler(Queue<ActionDTO>& from_server, std::atomic<bool>& stop_flag, GameView& game_view):
            from_server(from_server), stop_flag(stop_flag), game_view(game_view) {}

    void run() override {
        while (should_this_thread_keep_running()) {
            try {
                ActionDTO action_update;
                while (!from_server.try_pop(action_update)) {}
                if (action_update.type == ActionType::UNKNOWN)
                    break;  // Error
                game_view.update_graphics(action_update);
            } catch (...) {
                break;
            }
        }
        stop_flag = true;
    }

    bool should_this_thread_keep_running() const { return should_keep_running() && !stop_flag; }
};

#endif  // UPDATE_HANDLER_H
