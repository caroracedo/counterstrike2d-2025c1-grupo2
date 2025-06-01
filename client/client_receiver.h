#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

class Receiver: public Thread {
private:
    ClientProtocol& protocol;
    Queue<ActionDTO>& queue;

public:
    Receiver(ClientProtocol& protocol, Queue<ActionDTO>& queue): protocol(protocol), queue(queue) {}

    void run() override {
        while (should_keep_running()) {
            try {
                queue.push(protocol.receive_and_deserialize_updated_position());
            } catch (...) {
                break;
            }
        }
    }
};

#endif  // CLIENT_RECEIVER_H
