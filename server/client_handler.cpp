#include "client_handler.h"

#include <utility>

ClientHandler::ClientHandler(Socket&& socket, std::shared_ptr<Queue<ActionDTO>> initial_recv_queue,
                             uint16_t id):
        client_socket(std::move(socket)),
        protocol(this->client_socket, id),
        receiver(protocol, initial_recv_queue, stop_flag),
        sender(protocol, stop_flag),
        stop_flag(false),
        id(id) {}

void ClientHandler::run() {
    sender.start();
    receiver.start();
}

void ClientHandler::hard_kill() {
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

bool ClientHandler::is_alive() const { return receiver.is_alive() || sender.is_alive(); }

void ClientHandler::bind_queues(std::shared_ptr<Queue<ActionDTO>> recv_queue,
                                std::shared_ptr<Queue<ActionDTO>> send_queue) {
    receiver.bind_queue(recv_queue);
    sender.bind_queue(send_queue);
}
