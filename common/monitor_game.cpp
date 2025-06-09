#include "monitor_game.h"

MonitorGame::MonitorGame(Config& config): game(config) {}

bool MonitorGame::is_ready_to_start() {
    std::lock_guard<std::mutex> lock(mutex);
    return game.is_ready_to_start();
}

bool MonitorGame::is_over() {
    std::lock_guard<std::mutex> lock(mutex);
    return game.is_over();
}

void MonitorGame::start_round_game_phase() {
    std::lock_guard<std::mutex> lock(mutex);
    game.start_round_game_phase();
}

void MonitorGame::end_round_game_phase() {
    std::lock_guard<std::mutex> lock(mutex);
    game.end_round_game_phase();
}

void MonitorGame::switch_player_types() {
    std::lock_guard<std::mutex> lock(mutex);
    game.switch_player_types();
}

std::vector<std::shared_ptr<Object>> MonitorGame::get_objects() {
    std::lock_guard<std::mutex> lock(mutex);
    return game.get_objects();
}

void MonitorGame::add_player(PlayerType player_type, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    game.add_player(player_type, id);
}

bool MonitorGame::move(const Direction& direction, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.move(direction, id);
}

bool MonitorGame::shoot(const std::vector<uint16_t>& desired_position, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.shoot(desired_position, id);
}

bool MonitorGame::interact_with_bomb(uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.interact_with_bomb(id);
}

bool MonitorGame::shop_weapon(WeaponModel weapon, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.shop_weapon(weapon, id);
}

bool MonitorGame::shop_ammo(uint16_t ammo_amount, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.shop_ammo(ammo_amount, id);
}

bool MonitorGame::change_weapon(uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.change_weapon(id);
}
