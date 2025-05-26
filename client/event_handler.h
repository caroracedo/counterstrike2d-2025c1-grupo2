#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "mock_handler.h"

class EventHandler: public Thread {
private:
    Queue<ActionDTO>& to_server;
    MockHandler& mock_handler;
    std::atomic<bool>& stop_flag;

public:
    EventHandler(Queue<ActionDTO>& to_server, MockHandler& mock_handler,
                 std::atomic<bool>& stop_flag):
            to_server(to_server), mock_handler(mock_handler), stop_flag(stop_flag) {}

    void run() override {
        while (should_this_thread_keep_running()) {
            try {
                ActionDTO action = mock_handler.receive_and_parse_action();
                if (action.type == ActionType::QUIT || action.type == ActionType::UNKNOWN)
                    break;  // Error o salir
                to_server.push(action);
            } catch (...) {
                break;
            }
        }
        stop_flag = true;
    }

    bool should_this_thread_keep_running() const { return should_keep_running() && !stop_flag; }
};

#endif  // EVENT_HANDLER_H
