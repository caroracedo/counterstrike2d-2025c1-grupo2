#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>
#include <utility>
#include <vector>

#include "../common/monitor_games.h"
#include "../common/socket.h"
#include "../common/thread.h"

#include "client_handler.h"

class Acceptor: public Thread {
private:
    Socket server_socket;
    MonitorGames& monitor_games;
    std::list<ClientHandler*> client_handlers_list;

    void clear() {
        for (auto* client_handler: client_handlers_list) {
            // client_handler->hard_kill();
            client_handler->join();
            delete client_handler;
        }
        client_handlers_list.clear();
    }

    void reap() {
        client_handlers_list.remove_if([](auto* client_handler) {
            bool is_dead = client_handler->is_dead();
            if (is_dead) {
                client_handler->join();
                delete client_handler;
            }
            return is_dead;
        });
    }

public:
    /*
     * Constructor.
     **/
    Acceptor(const char* port, MonitorGames& monitor_games):
            server_socket(port), monitor_games(monitor_games) {}

    /* Override */
    void run() override {
        while (should_keep_running()) {
            try {
                Socket new_client_socket = server_socket.accept();
                ClientHandler* new_client_handler =
                        new ClientHandler(std::move(new_client_socket), monitor_games);
                reap();
                client_handlers_list.push_back(new_client_handler);
                new_client_handler->start();
            } catch (...) {
                break;
            }
        }
        clear();
    }

    void stop() override {
        Thread::stop();
        server_socket.shutdown(2);  // Cierra lectura y escritura
        server_socket.close();
    }
};

#endif  // ACCEPTOR_H


/*
#include "acceptor.h"

Acceptor::Acceptor(Socket& acceptor): acceptor(acceptor), matches(), _is_alive(true) {}

void Acceptor::run() {
    while (_is_alive) {
        try {
            Socket peer = acceptor.accept();
            if (!_is_alive)
                break;
            ClientHandler* c = new ClientHandler(std::move(peer), matches);
            reap();
            matches.reap();
            clients.push_back(c);
            c->start();
        } catch (const LibError& e) {
            if (!_is_alive) {
                break;
            }
        } catch (const std::runtime_error& e) {
            if (!_is_alive) {
                break;
            }
        }
    }
}

void Acceptor::stop() {
    if (!_is_alive) {
        return;
    }
    _is_alive = false;
    _keep_running = false;
    try {
        if (!acceptor.is_stream_recv_closed()) {
            acceptor.shutdown(2);
            acceptor.close();
        }
    } catch (...) {}
    for (auto it = clients.begin(); it != clients.end();) {
        if (!(*it)->is_dead()) {
            (*it)->hard_kill();
            (*it)->join();
        }
        delete *it;
        it = clients.erase(it);
    }
}

void Acceptor::reap() {
    auto it = clients.begin();
    while (it != clients.end()) {
        if ((*it)->is_dead()) {
            (*it)->join();
            delete *it;
            it = clients.erase(it);
        } else {
            ++it;
        }
    }
}

------------------------------------
void ClientHandler::run() {
    is_alive = keep_talking = true;
    while (keep_talking) {
        try {
            Command command = protocol.recv_command();
            switch (command) {
                case Command::CREATE:
                    create_match();
                    break;
                case Command::JOIN:
                    join_match();
                    break;
                case Command::LIST:
                    list_matches();
                    break;
                case Command::PLAY:
                    play();
                    break;
                default:
                    break;
            }
        } catch (const std::exception& e) {
            keep_talking = false;
            std::cout << "error: " << e.what() << std::endl;
        }
    }

    is_alive = false;
}

void ClientHandler::polite_kill() { keep_talking = false; }

void ClientHandler::hard_kill() {
    keep_talking = false;
    skt.shutdown(SHUT_RDWR);
    skt.close();
    match = nullptr; -> referencia al match
}

-------------------------------------------
class ServerProtocol {
    Socket* skt;

public:
    explicit ServerProtocol(Socket& skt);
---
ServerProtocol::ServerProtocol(Socket& skt): skt(&skt) {}

*/
