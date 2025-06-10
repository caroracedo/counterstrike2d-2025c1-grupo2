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
    std::shared_ptr<Queue<ActionDTO>> add_queue_to(uint16_t client_id);
    void send_update(const ActionDTO& update);
};

#endif  // MONITOR_CLIENT_SEND_QUEUES_H
