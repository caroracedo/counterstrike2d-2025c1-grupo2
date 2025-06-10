#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

class ClientReceiver: public Thread {
private:
    ClientProtocol& protocol;
    Queue<ActionDTO>& recv_queue;

public:
    ClientReceiver(ClientProtocol& protocol, Queue<ActionDTO>& recv_queue);

    void run() override;
};

#endif  // CLIENT_RECEIVER_H
