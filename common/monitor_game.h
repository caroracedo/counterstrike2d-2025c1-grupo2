#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "game.h"
#include "queue.h"

class MonitorGame {
private:
    Game game;
    std::mutex mutex;

public:
    explicit MonitorGame(Config& config): game(config) {}

    /* Inicialización */
    size_t is_ready_to_start() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.is_ready_to_start();
    }

    /* Finalización */
    bool is_over() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.is_over();
    }

    /* Snapshot */
    std::vector<std::shared_ptr<Object>> get_objects() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.get_objects();
    }

    /* Funcionalidades */
    /* Agregar jugador */
    void add_player(PlayerType player_type, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        game.add_player(player_type, id);
    }

    /* Mover */
    bool move(const Direction& direction, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.move(direction, id);
    }

    /* Disparar */
    bool shoot(const std::vector<uint16_t>& desired_position, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.shoot(desired_position, id);
    }
};

#endif  // MONITOR_GAME_H
