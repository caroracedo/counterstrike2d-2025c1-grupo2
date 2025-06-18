#include "acceptor.h"

#include <utility>

/* Constructor */
Acceptor::Acceptor(const char* yaml_path):
        config(yaml_path),
        server_socket(config.get_server_port().c_str()),
        matches_monitor(config),
        next_id(0) {}

/* Limpieza */
void Acceptor::kill_client_handler(ClientHandler* client_handler) {
    client_handler->hard_kill();
    client_handler->join();
    delete client_handler;
}

void Acceptor::clear() {
    for (auto* client_handler: client_handlers_list) {
        kill_client_handler(client_handler);
    }
    client_handlers_list.clear();
    matches_monitor.clear();
}

void Acceptor::reap() {
    client_handlers_list.remove_if([this](ClientHandler* client_handler) {
        if (!client_handler->is_alive()) {
            kill_client_handler(client_handler);
            return true;
        }
        return false;
    });
    matches_monitor.reap();
}

/* Override */
void Acceptor::run() {
    while (should_keep_running()) {
        try {
            Socket new_client_socket = server_socket.accept();
            ClientHandler* new_client_handler = new ClientHandler(
                    std::move(new_client_socket), matches_monitor, map_loader, ++next_id);
            new_client_handler->start();
            client_handlers_list.push_back(new_client_handler);
            reap();
        } catch (...) {}
    }
    clear();
}

void Acceptor::stop() {
    Thread::stop();
    server_socket.shutdown(2);
}
