#include "monitor_client_send_queues.h"

#include <iostream>

/* Agregar Queue */
void MonitorClientSendQueues::add_queue_to(std::shared_ptr<Queue<ActionDTO>> client_send_queue,
                                           uint16_t client_id) {
    std::lock_guard<std::mutex> lock(mutex);
    client_send_queues.try_emplace(client_id, client_send_queue);
}

/* Enviar Update */
void MonitorClientSendQueues::send_update(const ActionDTO& update) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto it = client_send_queues.begin(); it != client_send_queues.end();) {
        try {
            it->second->try_push(update);
            ++it;
        } catch (...) {
            it = client_send_queues.erase(it);
        }
    }
}
