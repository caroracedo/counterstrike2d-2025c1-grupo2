#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <atomic>
#include <memory>

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#include "server_protocol.h"
#include "server_receiver.h"
#include "server_sender.h"

class ClientHandler: public Thread {
private:
    /* Configuración */
    Socket client_socket;
    ServerProtocol protocol;
    ServerReceiver receiver;
    ServerSender sender;
    uint16_t id;
    std::atomic<bool> stop_flag = false;

public:
    /* Constructor */
    ClientHandler(Socket&& socket, std::shared_ptr<Queue<ActionDTO>> initial_recv_queue,
                  std::shared_ptr<Queue<ActionDTO>> initial_send_queue, uint16_t id);

    /* Override */
    void run() override;
    bool is_alive() const override;

    /* Cierre */
    void hard_kill();

    /* Vinculación */
    void bind_queue(std::shared_ptr<Queue<ActionDTO>> recv_queue);
};

#endif  // CLIENT_HANDLER_H
