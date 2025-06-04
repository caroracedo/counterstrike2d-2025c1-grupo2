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
// #define ROUNDS 10
#define ROUNDS 1

class GameLoop: public Thread {
private:
    Config config;
    MonitorGame monitor_game;
    Acceptor acceptor;
    Queue<ActionDTO> recv_queue;
    MonitorClientSendQueues monitor_client_send_queues;

    bool do_action(const ActionDTO& action_dto) {
        switch (action_dto.type) {
            case ActionType::PLAYERTYPE:
                monitor_game.add_player(action_dto.player_type, action_dto.id);
                return true;
            case ActionType::MOVE:
                return monitor_game.move(action_dto.direction, action_dto.id);
            case ActionType::SHOOT:
                return monitor_game.shoot(action_dto.desired_position, action_dto.id);
            case ActionType::BOMB:
                monitor_game.plant_bomb(action_dto.id);
                return true;
            default:
                return false;
        }
    }

    void send_snapshot_to_all_clients() {
        ActionDTO update = {ActionType::UPDATE, process_objects(monitor_game.get_objects())};
        monitor_client_send_queues.send_update(update);
    }

    void send_end_to_all_clients() {
        monitor_client_send_queues.send_update(ActionDTO(ActionType::END));
    }

    std::vector<ObjectDTO> process_objects(const std::vector<std::shared_ptr<Object>>& objects) {
        std::vector<ObjectDTO> object_dtos;
        object_dtos.reserve(objects.size());
        std::transform(objects.begin(), objects.end(), std::back_inserter(object_dtos),
                       [](const std::shared_ptr<Object>& obj_ptr) { return obj_ptr->get_dto(); });
        return object_dtos;
    }

    void waiting_lobby() {
        while (!monitor_game.is_ready_to_start() && should_keep_running()) {
            ActionDTO action;
            if (recv_queue.try_pop(action)) {
                do_action(action);
            }
        }
        std::cout << "¡Que comience el juego!" << std::endl;
    }

    void game_loop() {
        auto snapshot_interval = std::chrono::milliseconds(33); // ~30FPS

        for (size_t round = 0; round < ROUNDS && should_keep_running(); ++round) {
            // monitor_game.start_new_round(round);  // Inicializa lógica de la ronda

            auto last_snapshot_time = std::chrono::steady_clock::now();

            while (!monitor_game.is_over() && should_keep_running()) {
                ActionDTO action;
                if (recv_queue.try_pop(action)) {
                    do_action(action);
                }

                auto now = std::chrono::steady_clock::now();
                if (now - last_snapshot_time >= snapshot_interval) {
                    send_snapshot_to_all_clients();
                    last_snapshot_time = now;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            // send_snapshot_to_all_clients();  // Snapshot final al terminar la ronda
            // monitor_game.end_round(round);  // Lógica de cierre de ronda
        }
        send_end_to_all_clients();
    }

public:
    explicit GameLoop(const char* yaml_path):
            config(yaml_path),
            monitor_game(config),
            acceptor(config, recv_queue, monitor_client_send_queues, monitor_game) {}

    void run() override {
        acceptor.start();

        waiting_lobby();
        game_loop();

        acceptor.stop();
        acceptor.join();
    }
};

#endif  // GAME_LOOP_H
