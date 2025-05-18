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
        if (!game.move(direction))
            return false;
        return true;
    }

    /* Mover */
    std::vector<uint16_t> get_position() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.get_position();
    }
};

#endif  // MONITOR_GAME_H
