#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <condition_variable>
#include <iostream>
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
    /* Mover */
    bool move(Direction direction) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.move(direction);
    }

    std::pair<bool, std::vector<uint16_t>> shoot(const std::vector<uint16_t>& position,
                                                 Direction direction, WeaponDTO weapon) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.shoot(position, direction, weapon);
    }

    /* Mover */
    std::vector<uint16_t> get_position() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.get_position();
    }
};

#endif  // MONITOR_GAME_H
