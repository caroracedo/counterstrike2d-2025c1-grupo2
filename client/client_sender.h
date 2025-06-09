#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

class ClientSender: public Thread {
private:
    ClientProtocol& protocol;
    Queue<ActionDTO>& send_queue;

public:
    ClientSender(ClientProtocol& protocol, Queue<ActionDTO>& send_queue);

    void run() override;
    void stop() override;
};

#endif  // CLIENT_SENDER_H
