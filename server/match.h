#ifndef MATCH_H
#define MATCH_H

#include <chrono>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "../common/action_DTO.h"
#include "../common/object_DTO.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "monitor_client_send_queues.h"
#include "monitor_game.h"

class Match: public Thread {
private:
    Config& config;
    Map map;
    MonitorGame monitor_game;
    std::shared_ptr<Queue<ActionDTO>> recv_queue;
    std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues;

    bool do_action(const ActionDTO& action_dto);
    bool do_shop_action(const ActionDTO& action_dto);
    bool do_start_action(const ActionDTO& action_dto);
    void send_initial_snapshot_to_all_clients();
    void send_snapshot_to_all_clients();
    void send_shop_to_all_clients();
    void send_end_to_all_clients();
    void send_stats_to_all_clients();
    std::vector<ObjectDTO> process_objects(const std::vector<std::shared_ptr<Object>>& objects);
    std::vector<ObjectDTO> process_dynamic_objects(
            const std::vector<std::shared_ptr<Object>>& objects);
    void waiting_phase();
    void shopping_phase();
    void game_phase();
    void stats_phase();

public:
    explicit Match(Config& config, std::shared_ptr<Queue<ActionDTO>> recv_queue,
                   std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues,
                   const std::string& map_str);

    void run() override;
    void add_player(const ActionDTO& action_dto);
    TerrainType get_terrain() { return map.get_terrain(); }
};

#endif  // MATCH_H
