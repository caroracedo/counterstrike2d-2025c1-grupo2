#ifndef MONITOR_GAMES_H
#define MONITOR_GAMES_H

#include <algorithm>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "monitor_game.h"

class MonitorGames {
private:
    std::map<std::string, MonitorGame*> games;
    std::mutex mutex;

public:
    /* Mover */
    bool move_game(const std::string& game_name, Direction direction) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!games.count(game_name))
            return false;
        return games[game_name]->move(direction);
    }

    /* Getters */
    std::vector<uint8_t> get_position_game(const std::string& game_name) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!games.count(game_name))
            return {};
        return games[game_name]->get_position();
    }

    /*
     * Destructor.
     **/
    ~MonitorGames() {
        for (auto& pair: games) {
            delete pair.second;
        }
        games.clear();
    }
};

#endif  // MONITOR_GAMES_H
