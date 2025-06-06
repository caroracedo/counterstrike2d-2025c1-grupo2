#ifndef MONITOR_CLIENT_SEND_QUEUES_H
#define MONITOR_CLIENT_SEND_QUEUES_H

#include <map>
#include <memory>
#include <mutex>

#include "../common/action_DTO.h"
#include "../common/queue.h"

class MonitorClientSendQueues {
    std::map<uint16_t, std::shared_ptr<Queue<ActionDTO>>> client_send_queues;
    std::mutex mutex;

public:
    std::shared_ptr<Queue<ActionDTO>> add_queue_to(uint16_t client_id) {
        std::lock_guard<std::mutex> lock(mutex);
        auto [it, inserted] =
                client_send_queues.try_emplace(client_id, std::make_shared<Queue<ActionDTO>>());
        return it->second;
    }

    void send_update(const ActionDTO& update) {
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
};

#endif  // MONITOR_CLIENT_SEND_QUEUES_H
