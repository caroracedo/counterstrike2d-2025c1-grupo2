#include "client_receiver.h"

ClientReceiver::ClientReceiver(ClientProtocol& protocol, Queue<ActionDTO>& recv_queue):
        protocol(protocol), recv_queue(recv_queue) {}

void ClientReceiver::run() {
    while (should_keep_running()) {
        try {
            recv_queue.push(protocol.receive_and_deserialize_action());
        } catch (...) {
            break;
        }
    }
}
