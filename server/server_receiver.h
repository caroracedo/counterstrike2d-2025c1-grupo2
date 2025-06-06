#ifndef SERVER_RECEIVER_H
#define SERVER_RECEIVER_H

#include <memory>

#include "../common/queue.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ServerReceiver: public Thread {
private:
    ServerProtocol& protocol;
    std::shared_ptr<Queue<ActionDTO>> recv_queue;
    std::atomic<bool>& stop_flag;

public:
    ServerReceiver(ServerProtocol& protocol, std::shared_ptr<Queue<ActionDTO>> recv_queue,
                   std::atomic<bool>& stop_flag):
            protocol(protocol), recv_queue(recv_queue), stop_flag(stop_flag) {}

    void run() override {
        while (should_this_thread_keep_running()) {
            try {
                ActionDTO action = protocol.receive_and_deserialize_action();
                if (action.type == ActionType::UNKNOWN)
                    break;
                recv_queue->push(action);
            } catch (...) {
                break;
            }
        }
        stop_flag = true;
        stop();
    }

    bool should_this_thread_keep_running() { return should_keep_running() && !stop_flag; }

    void stop() override {
        if (should_keep_running()) {
            Thread::stop();
        }
    }

    void bind_queue(std::shared_ptr<Queue<ActionDTO>> new_recv_queue) {
        recv_queue = new_recv_queue;
    }
};

#endif  // SERVER_RECEIVER_H
