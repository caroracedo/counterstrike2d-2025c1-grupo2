#ifndef MONITOR_CLIENT_SEND_QUEUES_H
#define MONITOR_CLIENT_SEND_QUEUES_H

#include <map>
#include <mutex>

#include "../common/action_DTO.h"
#include "../common/queue.h"

class MonitorClientSendQueues {
    std::map<uint16_t, Queue<ActionDTO>*> client_send_queues;
    std::mutex mutex;

    void remove(uint16_t client_id) {
        std::lock_guard<std::mutex> lock(mutex);
        client_send_queues.erase(client_id);
    }

public:
    Queue<ActionDTO>* add_queue_to(uint16_t client_id) {
        std::lock_guard<std::mutex> lock(mutex);
        client_send_queues[client_id] = new Queue<ActionDTO>();
        return client_send_queues[client_id];
    }

    void send_update(const ActionDTO& update) {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto it = client_send_queues.begin(); it != client_send_queues.end();) {
            try {
                it->second->try_push(update);
                ++it;
            } catch (...) {
                remove(it->first);
                it = client_send_queues.erase(it);
            }
        }
    }
};

#endif  // MONITOR_CLIENT_SEND_QUEUES_H
