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
    bool move(Direction direction, const uint16_t& id) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.move(direction, id);
    }
    std::vector<ObjectDTO> get_objects() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.get_objects();
    }
};

#endif  // MONITOR_GAME_H
