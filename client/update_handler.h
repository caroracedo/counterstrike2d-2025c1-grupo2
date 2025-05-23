#ifndef UPDATE_HANDLER_H
#define UPDATE_HANDLER_H

#include <atomic>

#include "../common/queue.h"
#include "../common/thread.h"

#include "mock_handler.h"

class UpdateHandler: public Thread {
private:
    Queue<ActionDTO>& from_server;
    MockHandler& mock_handler;
    std::atomic<bool>& stop_flag;

public:
    UpdateHandler(Queue<ActionDTO>& from_server, MockHandler& mock_handler,
                  std::atomic<bool>& stop_flag):
            from_server(from_server), mock_handler(mock_handler), stop_flag(stop_flag) {}

    void run() override {
        while (should_handler_keep_running()) {
            try {
                ActionDTO action_update;
                while (!from_server.try_pop(action_update)) {}
                if (action_update.type == ActionType::UNKNOWN ||
                    !mock_handler.update_graphics(action_update))
                    break;  // Error
            } catch (...) {
                break;
            }
        }
        stop_flag = true;
    }

    bool should_handler_keep_running() const { return should_keep_running() && !stop_flag; }
};

#endif  // UPDATE_HANDLER_H
