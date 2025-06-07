#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <memory>

#include "../common/queue.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ServerSender: public Thread {
private:
    ServerProtocol& protocol;
    std::shared_ptr<Queue<ActionDTO>> send_queue;
    std::atomic<bool>& stop_flag;

public:
    ServerSender(ServerProtocol& protocol, std::atomic<bool>& stop_flag):
            protocol(protocol), stop_flag(stop_flag) {}

    void run() override {
        while (should_this_thread_keep_running()) {
            try {
                if (send_queue && !protocol.serialize_and_send_action(send_queue->pop()))
                    break;
            } catch (...) {
                break;
            }
        }
        stop_flag = true;
        stop();
    }

    void stop() override {
        Thread::stop();
        try {
            if (send_queue)
                send_queue->close();
        } catch (const std::runtime_error& e) {}
    }

    bool should_this_thread_keep_running() { return should_keep_running() && !stop_flag; }

    void bind_queue(std::shared_ptr<Queue<ActionDTO>> new_send_queue) {
        send_queue = new_send_queue;
    }
};

#endif  // SERVER_SENDER_H
