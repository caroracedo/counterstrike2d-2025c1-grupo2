#include "match.h"

#include <algorithm>
#include <iostream>

#define SHOPPING_TIME 10  // 10 segundos para shopping
#define SNAPSHOT_TIME 33  // ~30FPS

Match::Match(Config& config, std::shared_ptr<Queue<ActionDTO>> recv_queue,
             std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues):
        config(config),
        monitor_game(config),
        recv_queue(recv_queue),
        monitor_client_send_queues(monitor_client_send_queues) {}

bool Match::do_action(const ActionDTO& action_dto) {
    switch (action_dto.type) {
        case ActionType::MOVE:
            return monitor_game.move(action_dto.direction, action_dto.id);
        case ActionType::SHOOT:
            return monitor_game.shoot(action_dto.desired_position, action_dto.id);
        case ActionType::BOMB:
            return monitor_game.interact_with_bomb(action_dto.id);
        case ActionType::CHANGE:
            return monitor_game.change_weapon(action_dto.id);
        default:
            return false;
    }
}

bool Match::do_shop_action(const ActionDTO& action_dto) {
    switch (action_dto.type) {
        case ActionType::WEAPON:
            return monitor_game.shop_weapon(action_dto.weapon, action_dto.id);
        case ActionType::AMMOPRIMARY:
        case ActionType::AMMOSECONDARY:
            return monitor_game.shop_ammo(action_dto.ammo, action_dto.weapon_type, action_dto.id);
        default:
            return false;
    }
}

void Match::send_snapshot_to_all_clients() {
    monitor_client_send_queues->send_update(
            {ActionType::UPDATE, process_objects(monitor_game.get_objects())});
}

void Match::send_shop_to_all_clients() {
    monitor_client_send_queues->send_update({ActionType::SHOP, config.get_weapons()});
}

void Match::send_end_to_all_clients() {
    monitor_client_send_queues->send_update(ActionDTO(ActionType::END));
}

std::vector<ObjectDTO> Match::process_objects(const std::vector<std::shared_ptr<Object>>& objects) {
    std::vector<ObjectDTO> object_dtos;
    object_dtos.reserve(objects.size());
    std::transform(objects.begin(), objects.end(), std::back_inserter(object_dtos),
                   [](const std::shared_ptr<Object>& obj_ptr) { return obj_ptr->get_dto(); });
    return object_dtos;
}

void Match::waiting_phase() {
    std::unique_lock<std::mutex> lock(ready_mutex);
    std::cout << "[WAIT] Esperando a que todos los jugadores estén listos..." << std::endl;
    ready_cv.wait(lock,
                  [this]() { return monitor_game.is_ready_to_start() || !should_keep_running(); });
    send_snapshot_to_all_clients();
    std::cout << "[WAIT] ¡Todos los jugadores están listos!" << std::endl;
}

void Match::shopping_phase() {
    std::cout << "[SHOP] Iniciando fase de compras..." << std::endl;
    send_shop_to_all_clients();

    std::this_thread::sleep_for(std::chrono::seconds(SHOPPING_TIME));

    ActionDTO action;
    try {
        while (recv_queue->try_pop(action)) {
            do_shop_action(action);
        }
    } catch (const ClosedQueue&) {
        stop();
    }

    std::cout << "[SHOP] Terminando fase de compras..." << std::endl;
}

void Match::game_phase(std::chrono::_V2::steady_clock::time_point last_snapshot_time) {
    std::cout << "[GAME] Iniciando fase de juego..." << std::endl;
    send_snapshot_to_all_clients();
    monitor_game.start_round_game_phase();

    auto snapshot_interval = std::chrono::milliseconds(SNAPSHOT_TIME);
    while (!monitor_game.is_over() && should_keep_running()) {
        auto start = std::chrono::steady_clock::now();
        ActionDTO action;
        try {
            if (recv_queue->try_pop(action)) {
                do_action(action);
            }
        } catch (const ClosedQueue&) {
            stop();
        }

        auto now = std::chrono::steady_clock::now();
        if (now - last_snapshot_time >= snapshot_interval) {
            send_snapshot_to_all_clients();
            last_snapshot_time = now;
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = end - start;
        if (elapsed < snapshot_interval)
            std::this_thread::sleep_for(snapshot_interval - elapsed);
    }

    send_snapshot_to_all_clients();
    monitor_game.end_round_game_phase();
    std::cout << "[GAME] Terminando fase de juego..." << std::endl;
}

void Match::run() {
    waiting_phase();

    std::cout << "[MATCH] ¡Que comience la partida!" << std::endl;

    for (size_t round = 1; round <= config.get_rounds_total() && should_keep_running(); ++round) {
        std::cout << "[ROUND] Iniciando ronda..." << std::endl;
        auto last_snapshot_time = std::chrono::steady_clock::now();
        shopping_phase();
        game_phase(last_snapshot_time);
        std::cout << "[ROUND] Terminando ronda..." << std::endl;
        if (round == config.get_rounds_total() / 2)
            monitor_game.switch_player_types();
    }

    send_end_to_all_clients();
    std::cout << "[MATCH] ¡La partida ha finalizado!" << std::endl;
}

// Nota: En la waiting phase se verifica si están conectados los jugadores necesarios para iniciar
// la partida o si la partida fue interrumpida. Esta condición se notifica al agregar un jugador o
// al detener la partida.

void Match::add_player(const ActionDTO& action_dto) {
    {
        std::lock_guard<std::mutex> lock(ready_mutex);
        monitor_game.add_player(action_dto.player_type, action_dto.id);
    }
    ready_cv.notify_all();
}

void Match::stop() {
    Thread::stop();
    ready_cv.notify_all();
}
