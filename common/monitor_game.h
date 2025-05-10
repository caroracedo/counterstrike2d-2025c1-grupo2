#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <condition_variable>
#include <mutex>
#include <string>

#include "../common_src/game.h"

class MonitorGame {
private:
    Game game;
    std::mutex mutex;
    std::condition_variable is_turn;
    bool is_full = false;

public:
    /* Mover */
    bool move_game(Turn turn, Direction direction) {
        std::unique_lock<std::mutex> lock(mutex);
        if (!game.move(direction))
            return false;
        is_turn.notify_all();
        while (game.get_current_turn() != turn && game.get_state() == State::IN_PROGRESS)
            is_turn.wait(lock);
        return true;
    }

    /* Ingresar */
    bool join_game(Turn turn) {
        std::unique_lock<std::mutex> lock(mutex);
        if (is_full)
            return false;
        is_full = true;
        while (game.get_current_turn() != turn) is_turn.wait(lock);
        return true;
    }
};

#endif  // MONITOR_GAME_H
