#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

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
    ServerSender sender;
    ServerReceiver receiver;
    uint16_t id;

public:
    ClientHandler(Socket&& socket, Queue<ActionDTO>& recv_queue, Queue<ActionDTO>* send_queue,
                  uint16_t id):
            client_socket(std::move(socket)),
            protocol(this->client_socket, id),
            sender(protocol, send_queue),
            receiver(protocol, recv_queue),
            id(id) {}

    void run() override {
        sender.start();
        receiver.start();
    }

    void hard_kill() {
        Thread::stop();

        sender.stop();
        receiver.stop();
        sender.join();
        receiver.join();

        client_socket.shutdown(2);  // Cierra lectura y escritura
        client_socket.close();
    }

    bool is_alive() const override { return receiver.is_alive() || sender.is_alive(); }
};

#endif  // CLIENT_HANDLER_H
