#ifndef SERVER_RECEIVER_H
#define SERVER_RECEIVER_H

#include <atomic>
#include <memory>

#include "../common/queue.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ServerReceiver: public Thread {
private:
    ServerProtocol& protocol;
    std::shared_ptr<Queue<ActionDTO>> recv_queue;
    std::atomic<bool>& stop_flag;

public:
    ServerReceiver(ServerProtocol& protocol, std::shared_ptr<Queue<ActionDTO>> recv_queue,
                   std::atomic<bool>& stop_flag);

    void run() override;
    void stop() override;
    bool should_this_thread_keep_running();
    void bind_queue(std::shared_ptr<Queue<ActionDTO>> new_recv_queue);
};

#endif  // SERVER_RECEIVER_H
