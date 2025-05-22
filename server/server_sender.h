#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ServerSender: public Thread {
private:
    ServerProtocol& protocol;
    Queue<ActionDTO>& send_queue;

public:
    ServerSender(ServerProtocol& protocol, Queue<ActionDTO>& send_queue):
            protocol(protocol), send_queue(send_queue) {}

    void run() override {
        while (should_keep_running()) {
            try {
                protocol.serialize_and_send_updated_game(send_queue.pop());
            } catch (...) {
                break;
            }
        }
    }

    void stop() override {
        Thread::stop();
        send_queue.close();
    }
};

#endif  // SERVER_SENDER_H
