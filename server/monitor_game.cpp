#include "monitor_game.h"

/* Constructor */
MonitorGame::MonitorGame(Config& config, Map& map, uint16_t number_terrorist,
                         uint16_t number_counterterrorist):
        game(config, map, number_terrorist, number_counterterrorist) {}

/* Agregar jugador */
void MonitorGame::add_player(PlayerType player_type, PlayerSkin player_skin, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    game.add_player(player_type, player_skin, id);
}

/* Validación */
bool MonitorGame::is_ready_to_start() {
    std::lock_guard<std::mutex> lock(mutex);
    return game.is_ready_to_start();
}

bool MonitorGame::is_over() {
    std::lock_guard<std::mutex> lock(mutex);
    return game.is_over();
}

/* Rondas */
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

/* Mover */
bool MonitorGame::move(const Direction& direction, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.move(direction, id);
}

/* Disparar */
bool MonitorGame::shoot(const std::vector<uint16_t>& desired_position, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.shoot(desired_position, id);
}

/* Interactuar con bomba */
bool MonitorGame::interact_with_bomb(uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.interact_with_bomb(id);
}

/* Comprar arma */
bool MonitorGame::shop_weapon(WeaponModel weapon, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.shop_weapon(weapon, id);
}

/* Comprar munición */
bool MonitorGame::shop_ammo(uint16_t ammo_amount, WeaponType weapon, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.shop_ammo(ammo_amount, weapon, id);
}

/* Cambiar arma */
bool MonitorGame::change_weapon(uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    return game.change_weapon(id);
}

/* Tomar arma */
void MonitorGame::pick_up_weapon(uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    game.pick_up_weapon(id);
}

/* Rotar */
void MonitorGame::rotate(float angle, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    game.rotate_player(angle, id);
}

/* Salir */
void MonitorGame::quit(uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    game.quit(id);
}

/* Setters */
void MonitorGame::set_ready_to_start() {
    std::lock_guard<std::mutex> lock(mutex);
    game.set_ready_to_start();
}

/* Getters */
std::vector<std::shared_ptr<Object>> MonitorGame::get_objects() {
    std::lock_guard<std::mutex> lock(mutex);
    return game.get_objects();
}

Stats MonitorGame::get_stats() {
    std::lock_guard<std::mutex> lock(mutex);
    return game.get_stats();
}
