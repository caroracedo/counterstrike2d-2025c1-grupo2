#ifndef CLIENT_H
#define CLIENT_H

#include "common/action_DTO.h"
#include "graphics/game_view.h"
#include "graphics/input_handler.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"

class Client {
private:
    Queue<ActionDTO> send_queue;
    Queue<ActionDTO> recv_queue;

    /* Ejecución */
    ActionDTO prelobby();
    ActionDTO lobby(ClientProtocol& protocol);
    void match_loop(InputHandler& input_handler, GameView& game_view);

public:
    /* Ejecución */
    void run();
};

#endif  // CLIENT_H
