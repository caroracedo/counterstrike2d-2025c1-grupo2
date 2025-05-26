#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ServerSender: public Thread {
private:
    ServerProtocol& protocol;
    Queue<ActionDTO>& send_queue;
    std::atomic<bool>& stop_flag;

public:
    ServerSender(ServerProtocol& protocol, Queue<ActionDTO>& send_queue,
                 std::atomic<bool>& stop_flag):
            protocol(protocol), send_queue(send_queue), stop_flag(stop_flag) {}

    void run() override {
        while (should_this_thread_keep_running()) {
            try {
                if (!protocol.serialize_and_send_updated_game(send_queue.pop()))
                    break;
            } catch (...) {
                break;
            }
        }
        stop();
    }

    void stop() override {
        if (should_keep_running()) {
            Thread::stop();
            send_queue.close();
        }
    }

    bool should_this_thread_keep_running() { return should_keep_running() && !stop_flag; }
};

#endif  // SERVER_SENDER_H
