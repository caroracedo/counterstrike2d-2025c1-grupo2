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

// #define ROUNDS 10
#define ROUNDS 1          // TODO: Desde config...
#define SHOPPING_TIME 10  // 10 segundos para shopping
#define SNAPSHOT_TIME 33  // ~30FPS

class GameLoop: public Thread {
private:
    Config config;
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
            case ActionType::BOMB:
                return monitor_game.interact_with_bomb(action_dto.id);
            default:
                return false;
        }
    }

    bool do_initial_action(const ActionDTO& action_dto) {
        if (action_dto.type != ActionType::PLAYERTYPE)
            return false;
        monitor_game.add_player(action_dto.player_type, action_dto.id);
        return true;
    }

    bool do_shop_action(const ActionDTO& action_dto) {
        switch (action_dto.type) {
            case ActionType::WEAPON:
                return monitor_game.shop_weapon(action_dto.weapon, action_dto.id);
            case ActionType::AMMO:
                return monitor_game.shop_ammo(action_dto.ammo, action_dto.id);
            default:
                return false;
        }
    }

    void send_snapshot_to_all_clients() {
        monitor_client_send_queues.send_update(
                {ActionType::UPDATE, process_objects(monitor_game.get_objects())});
    }

    void send_shop_to_all_clients() {
        monitor_client_send_queues.send_update({ActionType::SHOP, config.get_weapons()});
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
                do_initial_action(action);
            }
            send_snapshot_to_all_clients();
        }
    }

    void shopping_phase(std::chrono::_V2::steady_clock::time_point last_snapshot_time) {
        std::cout << "[SHOP] Iniciando fase de compras..." << std::endl;
        send_shop_to_all_clients();  // Mostrar las armas

        auto shopping_interval = std::chrono::seconds(SHOPPING_TIME);
        auto now = std::chrono::steady_clock::now();
        while (now - last_snapshot_time < shopping_interval) {
            ActionDTO action;
            if (recv_queue.try_pop(action)) {
                do_shop_action(action);
            }
            now = std::chrono::steady_clock::now();
        }

        std::cout << "[SHOP] Terminando fase de compras..." << std::endl;
    }

    void game_phase(std::chrono::_V2::steady_clock::time_point last_snapshot_time) {
        std::cout << "[GAME] Iniciando fase de juego..." << std::endl;
        send_snapshot_to_all_clients();  // Snapshot inicial
        monitor_game.start_round_game_phase();

        auto snapshot_interval = std::chrono::milliseconds(SNAPSHOT_TIME);
        while (!monitor_game.is_over() && should_keep_running()) {
            auto start = std::chrono::steady_clock::now();
            ActionDTO action;
            if (recv_queue.try_pop(action)) {
                do_action(action);
            }

            auto now = std::chrono::steady_clock::now();
            if (now - last_snapshot_time >= snapshot_interval) {
                send_snapshot_to_all_clients();
                last_snapshot_time = now;
            }

            // Dormir segundos faltantes...
            auto end = std::chrono::steady_clock::now();
            auto elapsed = end - start;
            if (elapsed < snapshot_interval)
                std::this_thread::sleep_for(snapshot_interval - elapsed);
        }

        send_snapshot_to_all_clients();  // Snapshot final
        monitor_game.end_round_game_phase();
        std::cout << "[GAME] Terminando fase de juego..." << std::endl;
    }

public:
    explicit GameLoop(const char* yaml_path):
            config(yaml_path),
            monitor_game(config),
            acceptor(config, recv_queue, monitor_client_send_queues, monitor_game) {}

    void run() override {
        acceptor.start();

        waiting_lobby();
        std::cout << "[GAME] ¡Que comience el juego!" << std::endl;

        for (size_t round = 1; round <= ROUNDS && should_keep_running(); ++round) {
            std::cout << "[ROUND] Iniciando ronda..." << std::endl;
            auto last_snapshot_time = std::chrono::steady_clock::now();
            shopping_phase(last_snapshot_time);
            game_phase(last_snapshot_time);
            std::cout << "[ROUND] Terminando ronda..." << std::endl;
            if (round == ROUNDS / 2)
                monitor_game.switch_player_types();
        }

        send_end_to_all_clients();
        std::cout << "[GAME] ¡El juego ha finalizado!" << std::endl;

        acceptor.stop();
        acceptor.join();
    }
};

#endif  // GAME_LOOP_H
