#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <utility>

#include "../common/action_DTO.h"
#include "../common/types.h"  // <-- Importa types.h

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"
#include "event_handler.h"
#include "mock_handler.h"
#include "update_handler.h"

class Client {
private:
    Socket client_socket;
    ClientProtocol protocol;

    Queue<ActionDTO> send_queue;
    Queue<ActionDTO> recv_queue;

    Sender sender;
    Receiver receiver;

    PlayerType player_type;

    PlayerType parse_player_type(const char* type_str) {
        std::string type(type_str);
        if (type == "Terrorist")
            return PlayerType::TERRORIST;
        return PlayerType::COUNTERTERRORIST;
    }

    void send_initial_configuration() {
        send_queue.push(ActionDTO{ActionType::PLAYERTYPE, player_type});
    }

public:
    Client(const char* hostname, const char* servname, const char* type_str):
            client_socket(hostname, servname),
            protocol(this->client_socket),
            sender(protocol, send_queue),
            receiver(protocol, recv_queue),
            player_type(parse_player_type(type_str)) {}

    void run() {
        sender.start();
        receiver.start();

        send_initial_configuration();

        MockHandler mock_handler;
        std::atomic<bool> stop_flag = false;
        EventHandler event_handler(send_queue, mock_handler, stop_flag);
        UpdateHandler update_handler(recv_queue, mock_handler, stop_flag);

        event_handler.start();
        update_handler.start();

        event_handler.join();
        update_handler.join();

        client_socket.shutdown(2);
        client_socket.close();

        sender.stop();
        receiver.stop();
        sender.join();
        receiver.join();
    }
};

#endif  // CLIENT_H
