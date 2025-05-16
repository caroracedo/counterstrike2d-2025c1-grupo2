#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

class Sender: public Thread {
private:
    ClientProtocol& protocol;
    Queue<ActionDTO>& queue;

public:
    Sender(ClientProtocol& protocol, Queue<ActionDTO>& queue): protocol(protocol), queue(queue) {}

    void run() override {
        while (should_keep_running()) {
            try {
                protocol.serialize_and_send_action(queue.pop());
            } catch (...) {
                break;
            }
        }
    }
};

#endif  // CLIENT_SENDER_H
