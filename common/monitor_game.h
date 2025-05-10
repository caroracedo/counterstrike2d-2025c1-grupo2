#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <condition_variable>
#include <mutex>
#include <string>
#include <utility>

#include "game.h"

class MonitorGame {
private:
    Game game;
    std::mutex mutex;

public:
    /* Mover */
    bool move(Direction direction) {
        std::unique_lock<std::mutex> lock(mutex);
        if (!game.move(direction))
            return false;
        return true;
    }

    /* Ingresar */
    bool join() {
        std::unique_lock<std::mutex> lock(mutex);
        return true;  // Por ahora no pienso en una cantidad específica de jugadores o cosas así...
    }
};

#endif  // MONITOR_GAME_H
