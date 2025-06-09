#include "server_sender.h"

ServerSender::ServerSender(ServerProtocol& protocol, std::atomic<bool>& stop_flag):
        protocol(protocol), stop_flag(stop_flag) {}

void ServerSender::run() {
    while (should_this_thread_keep_running()) {
        try {
            if (send_queue && !protocol.serialize_and_send_action(send_queue->pop()))
                break;
        } catch (...) {
            break;
        }
    }
    stop_flag = true;
    stop();
}

void ServerSender::stop() {
    Thread::stop();
    try {
        if (send_queue)
            send_queue->close();
    } catch (const std::runtime_error& e) {}
}

bool ServerSender::should_this_thread_keep_running() { return should_keep_running() && !stop_flag; }

void ServerSender::bind_queue(std::shared_ptr<Queue<ActionDTO>> new_send_queue) {
    send_queue = new_send_queue;
}
