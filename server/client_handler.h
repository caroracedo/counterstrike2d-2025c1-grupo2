#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <atomic>
#include <memory>
#include <utility>

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
                  uint16_t id):
            client_socket(std::move(socket)),
            protocol(this->client_socket, id),
            receiver(protocol, initial_recv_queue, stop_flag),
            sender(protocol, stop_flag),
            id(id) {}

    void run() override {
        sender.start();
        receiver.start();
    }

    void hard_kill() {
        Thread::stop();

        try {
            client_socket.shutdown(2);  // Cierra lectura y escritura
        } catch (...) {}
        client_socket.close();

        sender.stop();
        receiver.stop();

        sender.join();
        receiver.join();
    }

    bool is_alive() const override { return receiver.is_alive() || sender.is_alive(); }

    void bind_queues(std::shared_ptr<Queue<ActionDTO>> recv_queue,
                     std::shared_ptr<Queue<ActionDTO>> send_queue) {
        receiver.bind_queue(recv_queue);
        sender.bind_queue(send_queue);
    }
};

#endif  // CLIENT_HANDLER_H
