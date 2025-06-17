#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "common/queue.h"
#include "common/thread.h"

#include "client_protocol.h"

class ClientReceiver: public Thread {
private:
    /* Configuración */
    ClientProtocol& protocol;
    Queue<ActionDTO>& recv_queue;

public:
    /* Constructor */
    ClientReceiver(ClientProtocol& protocol, Queue<ActionDTO>& recv_queue);

    /* Override */
    void run() override;
};

#endif  // CLIENT_RECEIVER_H
