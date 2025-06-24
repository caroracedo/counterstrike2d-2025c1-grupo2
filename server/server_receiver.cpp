#include "server_receiver.h"

/* Constructor */
ServerReceiver::ServerReceiver(ServerProtocol& protocol, std::atomic<bool>& stop_flag):
        protocol(protocol), stop_flag(stop_flag) {}

/* Override */
void ServerReceiver::run() {
    while (should_this_thread_keep_running()) {
        try {
            ActionDTO action = protocol.receive_and_deserialize_action();
            if (action.type == ActionType::UNKNOWN) {
                break;
            }
            recv_queue->push(action);
        } catch (...) {
            break;
        }
    }
    stop_flag = true;
    stop();
}

/* Validación */
bool ServerReceiver::should_this_thread_keep_running() {
    return should_keep_running() && !stop_flag;
}

/* Setters */
void ServerReceiver::set_queue(std::shared_ptr<Queue<ActionDTO>> new_recv_queue) {
    recv_queue = new_recv_queue;
}
