#include "server_sender.h"

/* Constructor */
ServerSender::ServerSender(ServerProtocol& protocol, std::shared_ptr<Queue<ActionDTO>> send_queue,
                           std::atomic<bool>& stop_flag):
        protocol(protocol), send_queue(send_queue), stop_flag(stop_flag) {}

/* Override */
void ServerSender::run() {
    while (should_this_thread_keep_running()) {
        try {
            if (!protocol.serialize_and_send_action(send_queue->pop()))
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
        send_queue->close();
    } catch (const std::runtime_error& e) {}
}

/* Validación */
bool ServerSender::should_this_thread_keep_running() { return should_keep_running() && !stop_flag; }
