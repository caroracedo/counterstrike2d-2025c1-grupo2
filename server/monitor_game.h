#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "../common/game.h"
#include "../common/queue.h"

class MonitorGame {
private:
    Game game;
    std::mutex mutex;

public:
    explicit MonitorGame(Config& config, Map& map);

    bool is_ready_to_start();
    bool is_over();

    void start_round_game_phase();
    void end_round_game_phase();
    void switch_player_types();

    std::vector<std::shared_ptr<Object>> get_objects();

    void add_player(PlayerType player_type, uint16_t id);
    bool move(const Direction& direction, uint16_t id);
    bool shoot(const std::vector<uint16_t>& desired_position, uint16_t id);
    bool interact_with_bomb(uint16_t id);
    bool shop_weapon(WeaponModel weapon, uint16_t id);
    bool shop_ammo(uint16_t ammo_amount, WeaponType weapon, uint16_t id);
    bool change_weapon(uint16_t id);
    void pick_up_weapon(uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        game.pick_up_weapon(id);
    }
    void rotate(_Float16 angle, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        game.rotate_player(angle, id);
        (void)angle;
        (void)id;
    }
    void set_ready_to_start() { game.set_ready_to_start(); }

    Stats get_stats() const { return game.get_stats(); }
};

#endif  // MONITOR_GAME_H
