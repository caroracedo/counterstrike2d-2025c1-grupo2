#ifndef GAMES_MONITOR_H
#define GAMES_MONITOR_H

#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <algorithm>

#include "monitor_game.h"

class MonitorGames {
private:
    std::map<std::string, MonitorGame*> games;
    std::mutex mutex;

public:
    /* Crear */
    MonitorGame* create_game(const std::string& game_name) {
        std::lock_guard<std::mutex> lock(mutex);
        if (games.count(game_name))
            return nullptr;
        MonitorGame* game = new MonitorGame;
        games[game_name] = game;
        return game;
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
    MonitorGame* join_game(const std::string& game_name) {
        std::lock_guard<std::mutex> lock(mutex);
        if (games.count(game_name))
            return nullptr;
        return games[game_name];
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

#endif  // GAMES_MONITOR_H
