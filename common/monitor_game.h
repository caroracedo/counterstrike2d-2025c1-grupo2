#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <condition_variable>
#include <iostream>
#include <memory>
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
    explicit MonitorGame(Config& config): game(config) {}

    /* Verificaciones */
    bool is_ready_to_start() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.is_ready_to_start();
    }

    bool is_over() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.is_over();
    }

    /* Inicialización */
    void start_round_game_phase() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.start_round_game_phase();
    }

    /* Finalización */
    void end_round_game_phase() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.end_round_game_phase();
    }

    /* Switch de PlayerType */
    void switch_player_types() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.switch_player_types();
    }

    /* Snapshot */
    std::vector<std::shared_ptr<Object>> get_objects() {
        std::lock_guard<std::mutex> lock(mutex);
        return game.get_objects();
    }

    /* Funcionalidades */
    /* Agregar jugador */
    void add_player(PlayerType player_type, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        game.add_player(player_type, id);
    }

    /* Mover */
    bool move(const Direction& direction, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.move(direction, id);
    }

    /* Disparar */
    bool shoot(const std::vector<uint16_t>& desired_position, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.shoot(desired_position, id);
    }

    /* Plantar bomba */
    bool plant_bomb(uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.plant_bomb(id);
    }

    /* Compar Weapon */
    bool shop_weapon(WeaponModel weapon, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.shop_weapon(weapon, id);
    }

    /* Compar Ammo */
    bool shop_ammo(uint16_t ammo_amount, uint16_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        return game.shop_ammo(ammo_amount, id);
    }
};

#endif  // MONITOR_GAME_H
