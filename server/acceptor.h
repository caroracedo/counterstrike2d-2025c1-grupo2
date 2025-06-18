#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>
#include <memory>

#include "common/action_DTO.h"
#include "common/config.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#include "client_handler.h"
#include "map_loader.h"

class Acceptor: public Thread {
private:
    /* Configuración */
    Config config;
    MapLoader map_loader;
    Socket server_socket;
    std::list<ClientHandler*> client_handlers_list;
    MatchesMonitor matches_monitor;
    uint16_t next_id = 0;

    /* Limpieza */
    void kill_client_handler(ClientHandler* client_handler);
    void clear();
    void reap();

public:
    /* Constructor */
    explicit Acceptor(const char* yaml_path);

    /* Override */
    void run() override;
    void stop() override;
};

#endif  // ACCEPTOR_H
