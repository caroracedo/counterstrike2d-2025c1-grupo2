#include "client_sender.h"

ClientSender::ClientSender(ClientProtocol& protocol, Queue<ActionDTO>& send_queue):
        protocol(protocol), send_queue(send_queue) {}

void ClientSender::run() {
    while (should_keep_running()) {
        try {
            protocol.serialize_and_send_action(send_queue.pop());
        } catch (...) {
            break;
        }
    }
}

void ClientSender::stop() {
    Thread::stop();
    send_queue.close();
}
