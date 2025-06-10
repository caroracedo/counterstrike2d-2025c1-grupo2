#include "monitor_client_send_queues.h"

std::shared_ptr<Queue<ActionDTO>> MonitorClientSendQueues::add_queue_to(uint16_t client_id) {
    std::lock_guard<std::mutex> lock(mutex);
    auto [it, inserted] =
            client_send_queues.try_emplace(client_id, std::make_shared<Queue<ActionDTO>>());
    return it->second;
}

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
