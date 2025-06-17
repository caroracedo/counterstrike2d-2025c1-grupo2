#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <atomic>
#include <memory>

#include "common/queue.h"
#include "common/thread.h"

#include "server_protocol.h"

class ServerSender: public Thread {
private:
    /* Configuración */
    ServerProtocol& protocol;
    std::shared_ptr<Queue<ActionDTO>> send_queue;
    std::atomic<bool>& stop_flag;

public:
    /* Constructor */
    ServerSender(ServerProtocol& protocol, std::shared_ptr<Queue<ActionDTO>> send_queue,
                 std::atomic<bool>& stop_flag);

    /* Override */
    void run() override;
    void stop() override;

    /* Validación */
    bool should_this_thread_keep_running();
};

#endif  // SERVER_SENDER_H
