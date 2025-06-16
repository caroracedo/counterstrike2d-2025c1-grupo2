#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <atomic>
#include <memory>

#include "../common/queue.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ServerSender: public Thread {
private:
    ServerProtocol& protocol;
    std::shared_ptr<Queue<ActionDTO>> send_queue;
    std::atomic<bool>& stop_flag;

public:
    ServerSender(ServerProtocol& protocol, std::shared_ptr<Queue<ActionDTO>> send_queue,
                 std::atomic<bool>& stop_flag);

    void run() override;
    void stop() override;
    bool should_this_thread_keep_running();
    void bind_queue(std::shared_ptr<Queue<ActionDTO>> new_send_queue);
};

#endif  // SERVER_SENDER_H
