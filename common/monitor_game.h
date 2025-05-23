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
    MonitorGame(int width, int height): game(width, height) {}

    /* Mover */
    void move_object(int id, Direction direction) {
        std::lock_guard<std::mutex> lock(mutex);
        game.move_object(id, direction);
    }

    /* Snapshot */
    std::vector<ObjectDTO> get_snapshot() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.get_snapshot();
    }

    /* Agregar objeto */
    void add_player(int id) {
        std::lock_guard<std::mutex> lock(mutex);
        game.add_player(id);
    }
};

#endif  // MONITOR_GAME_H
