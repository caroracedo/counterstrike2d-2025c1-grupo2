#include "client_handler.h"

#include <utility>

/* Constructor */
ClientHandler::ClientHandler(Socket&& socket, std::shared_ptr<Queue<ActionDTO>> initial_recv_queue,
                             std::shared_ptr<Queue<ActionDTO>> initial_send_queue, uint16_t id):
        client_socket(std::move(socket)),
        protocol(client_socket, id),
        receiver(protocol, initial_recv_queue, stop_flag),
        sender(protocol, initial_send_queue, stop_flag),
        id(id) {}

/* Override */
void ClientHandler::run() {
    receiver.start();
    sender.start();
}

bool ClientHandler::is_alive() const { return receiver.is_alive() || sender.is_alive(); }

/* Cierre */
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

/* Vinculación */
void ClientHandler::bind_queue(std::shared_ptr<Queue<ActionDTO>> recv_queue) {
    receiver.bind_queue(recv_queue);
}
