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
                ActionDTO response = send_queue.pop();
                protocol.serialize_and_send_updated_position(response);
            } catch (...) {
                break;
            }
        }
    }
};

#endif  // SERVER_SENDER_H
