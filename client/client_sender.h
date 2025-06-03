#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

class Sender: public Thread {
private:
    ClientProtocol& protocol;
    Queue<ActionDTO>& send_queue;

public:
    Sender(ClientProtocol& protocol, Queue<ActionDTO>& send_queue):
            protocol(protocol), send_queue(send_queue) {}

    void run() override {
        while (should_keep_running()) {
            try {
                protocol.serialize_and_send_action(send_queue.pop());
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

#endif  // CLIENT_SENDER_H
