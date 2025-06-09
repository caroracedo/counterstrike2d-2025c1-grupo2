#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "game.h"
#include "queue.h"

class MonitorGame {
private:
    Game game;
    std::mutex mutex;

public:
    explicit MonitorGame(Config& config);

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
    bool shop_ammo(uint16_t ammo_amount, uint16_t id);
    bool change_weapon(uint16_t id);
};

#endif  // MONITOR_GAME_H
