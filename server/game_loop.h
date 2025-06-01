#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <algorithm>
#include <list>
#include <memory>
#include <vector>

#include "../common/action_DTO.h"
#include "../common/monitor_game.h"
#include "../common/queue.h"
#include "../common/thread.h"

#define MAX_POSITION 50

class GameLoop: public Thread {
private:
    MonitorGame monitor_game;
    Acceptor acceptor;
    Queue<ActionDTO> recv_queue;
    MonitorClientSendQueues monitor_client_send_queues;

    bool do_action(const ActionDTO& action_dto) {
        switch (action_dto.type) {
            case ActionType::MOVE:
                return monitor_game.move(action_dto.direction, action_dto.id);
            case ActionType::SHOOT:
                return monitor_game.shoot(action_dto.desired_position, action_dto.id);
            default:
                return false;
        }
    }

    void send_snapshot_to_all_clients() {
        ActionDTO update = {ActionType::UPDATE, process_objects(monitor_game.get_objects())};
        monitor_client_send_queues.send_update(update);
    }

    std::vector<ObjectDTO> process_objects(const std::vector<std::shared_ptr<Object>>& objects) {
        std::vector<ObjectDTO> object_dtos;
        object_dtos.reserve(objects.size());
        std::transform(objects.begin(), objects.end(), std::back_inserter(object_dtos),
                       [](const std::shared_ptr<Object>& obj_ptr) { return obj_ptr->get_dto(); });
        return object_dtos;
    }

public:
    explicit GameLoop(const char* port):
            acceptor(port, recv_queue, monitor_client_send_queues, monitor_game) {}

    void run() override {
        acceptor.start();

        using clock = std::chrono::steady_clock;
        auto last_snapshot_time = clock::now();
        auto interval = std::chrono::milliseconds(16);  // ~60 FPS

        while (should_keep_running()) {
            auto start = clock::now();

            ActionDTO action;
            if (recv_queue.try_pop(action))
                do_action(action);

            auto now = clock::now();
            if (now - last_snapshot_time >= interval) {
                send_snapshot_to_all_clients();
                last_snapshot_time = now;
            }

            // Esperar hasta completar 16 ms desde que empezó la iteración
            auto end = clock::now();
            auto elapsed = end - start;

            if (elapsed < interval)
                std::this_thread::sleep_for(interval - elapsed);
        }

        acceptor.stop();
        acceptor.join();
    }
};

#endif  // GAME_LOOP_H
