#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

class Receiver: public Thread {
private:
    ClientProtocol& protocol;
    Queue<ActionDTO>& recv_queue;

public:
    Receiver(ClientProtocol& protocol, Queue<ActionDTO>& recv_queue):
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

#endif  // CLIENT_RECEIVER_H
