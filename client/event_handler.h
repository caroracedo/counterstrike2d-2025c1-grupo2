#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "input_handler.h"

class EventHandler: public Thread {
private:
    Queue<ActionDTO>& send_queue;
    std::atomic<bool>& stop_flag;
    InputHandler& input_handler;

public:
    EventHandler(Queue<ActionDTO>& send_queue, std::atomic<bool>& stop_flag,
                 InputHandler& input_handler):
            send_queue(send_queue), stop_flag(stop_flag), input_handler(input_handler) {}

    void run() override {
        while (should_this_thread_keep_running()) {
            try {
                // afuera del Thread
                ActionDTO action = input_handler.receive_and_parse_action();
                if (action.type == ActionType::QUIT) {
                    break;
                } else if (action.type != ActionType::UNKNOWN) {
                    send_queue.push(action);
                }
            } catch (...) {
                break;
            }
        }
        stop_flag = true;
    }

    bool should_this_thread_keep_running() const { return should_keep_running() && !stop_flag; }
};

#endif  // EVENT_HANDLER_H
