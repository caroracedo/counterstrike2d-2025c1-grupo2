#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <atomic>
#include <memory>

#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"

#include "server_protocol.h"
#include "server_receiver.h"
#include "server_sender.h"

class ClientHandler: public Thread {
private:
    Socket client_socket;
    ServerProtocol protocol;
    ServerReceiver receiver;
    ServerSender sender;
    std::atomic<bool> stop_flag;
    uint16_t id;

public:
    ClientHandler(Socket&& socket, std::shared_ptr<Queue<ActionDTO>> initial_recv_queue,
                  uint16_t id);

    void run() override;
    void hard_kill();
    bool is_alive() const override;
    void bind_queues(std::shared_ptr<Queue<ActionDTO>> recv_queue,
                     std::shared_ptr<Queue<ActionDTO>> send_queue);
};

#endif  // CLIENT_HANDLER_H
