#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <atomic>
#include <memory>

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#include "map_loader.h"
#include "matches_monitor.h"
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
    MatchesMonitor& matches_monitor;
    MapLoader& map_loader;
    uint16_t id;
    std::atomic<bool> stop_flag = false;

public:
    /* Constructor */
    ClientHandler(Socket&& socket, MatchesMonitor& matches_monitor, MapLoader& map_loader,
                  uint16_t id);

    /* Override */
    void run() override;
    bool is_alive() const override;

    /* Cierre */
    void hard_kill();
};

#endif  // CLIENT_HANDLER_H
