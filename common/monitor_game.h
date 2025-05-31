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
    /* Agregar objeto */
    void add_player(uint16_t id, bool is_terrorist, bool has_bomb) {
        std::lock_guard<std::mutex> lock(mutex);
        game.add_player(id, is_terrorist, has_bomb);
    }

    /* Snapshot */
    std::vector<std::shared_ptr<Object>> get_objects() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.get_objects();
    }

    /* Funcionalidades */
    /* Mover */
    bool move(const Direction& direction, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.move(direction, id);
    }

    /* Disparar */
    bool shoot(const std::vector<uint16_t>& desired_position, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "Desired position: x=" << desired_position[0] << " y=" << desired_position[1]
                  << std::endl;
        return game.shoot(desired_position, id);
    }
};

#endif  // MONITOR_GAME_H
