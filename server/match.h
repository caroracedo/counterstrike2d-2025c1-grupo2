#ifndef MATCH_H
#define MATCH_H

#include <chrono>
#include <list>
#include <memory>
#include <thread>
#include <vector>

#include "../common/action_DTO.h"
#include "../common/monitor_game.h"
#include "../common/object_DTO.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "monitor_client_send_queues.h"

class Match: public Thread {
private:
    Config& config;
    MonitorGame monitor_game;
    std::shared_ptr<Queue<ActionDTO>> recv_queue;
    std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues;
    std::mutex ready_mutex;
    std::condition_variable ready_cv;

    bool do_action(const ActionDTO& action_dto);
    bool do_shop_action(const ActionDTO& action_dto);
    void send_snapshot_to_all_clients();
    void send_shop_to_all_clients();
    void send_end_to_all_clients();
    std::vector<ObjectDTO> process_objects(const std::vector<std::shared_ptr<Object>>& objects);
    void waiting_phase();
    void shopping_phase();
    void game_phase(std::chrono::_V2::steady_clock::time_point last_snapshot_time);

public:
    explicit Match(Config& config, std::shared_ptr<Queue<ActionDTO>> recv_queue,
                   std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues);

    void run() override;
    void add_player(const ActionDTO& action_dto);
    void stop() override;
};

#endif  // MATCH_H
