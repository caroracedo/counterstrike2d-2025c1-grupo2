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
    /* Crear */
    bool create_game(const std::string& game_name) {
        std::lock_guard<std::mutex> lock(mutex);
        if (games.count(game_name))
            return false;
        MonitorGame* game = new MonitorGame;
        games[game_name] = game;
        return true;
    }

    /* Listar */
    std::vector<std::string> list_games() {
        std::lock_guard<std::mutex> lock(mutex);
        std::vector<std::string> result;
        std::transform(games.begin(), games.end(), std::back_inserter(result),
                       [](const auto& pair) { return pair.first; });
        return result;
    }

    /* Unirse */
    bool join_game(const std::string& game_name) {
        std::lock_guard<std::mutex> lock(mutex);
        if (games.count(game_name))
            return false;
        return games[game_name]->join();
    }

    /* Mover */
    bool move_game(const std::string& game_name, Direction direction) {
        std::lock_guard<std::mutex> lock(mutex);
        if (games.count(game_name))
            return false;
        return games[game_name]->move(direction);
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
