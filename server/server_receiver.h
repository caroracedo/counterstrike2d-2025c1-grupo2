#ifndef SERVER_RECEIVER_H
#define SERVER_RECEIVER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ServerReceiver: public Thread {
private:
    ServerProtocol& protocol;
    Queue<ActionDTO>& recv_queue;

public:
    ServerReceiver(ServerProtocol& protocol, Queue<ActionDTO>& recv_queue):
            protocol(protocol), recv_queue(recv_queue) {}

    void run() override {
        while (should_keep_running()) {
            try {
                recv_queue.push(protocol.receive_and_deserialize_action());
            } catch (...) {
                break;
            }
        }
    }
};

#endif  // SERVER_RECEIVER_H
