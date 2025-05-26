#ifndef MONITOR_CLIENT_SEND_QUEUES_H
#define MONITOR_CLIENT_SEND_QUEUES_H

#include <map>
#include <mutex>

#include "../common/action_DTO.h"
#include "../common/queue.h"

class MonitorClientSendQueues {
    std::map<uint16_t, Queue<ActionDTO>> client_send_queues;
    std::mutex mutex;

public:
    Queue<ActionDTO>& add_queue_to(uint16_t client_id) {
        std::lock_guard<std::mutex> lock(mutex);
        auto [it, _] = client_send_queues.try_emplace(client_id);
        return it->second;
    }

    void send_update(const ActionDTO& update) {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto it = client_send_queues.begin(); it != client_send_queues.end();) {
            try {
                it->second.try_push(update);
                ++it;
            } catch (...) {
                it = client_send_queues.erase(it);
            }
        }
    }
};

#endif  // MONITOR_CLIENT_SEND_QUEUES_H
