#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "common/game.h"

class MonitorGame {
private:
    /* Configuration */
    Game game;
    std::mutex mutex;

public:
    /* Constructor */
    explicit MonitorGame(Config& config, Map& map);

    /* Agregar jugador */
    void add_player(PlayerType player_type, uint16_t id);

    /* Validación */
    bool is_ready_to_start();
    bool is_over();

    /* Rondas */
    void start_round_game_phase();
    void end_round_game_phase();
    void switch_player_types();

    /* Mover */
    bool move(const Direction& direction, uint16_t id);

    /* Disparar */
    bool shoot(const std::vector<uint16_t>& desired_position, uint16_t id);

    /* Interactuar con bomba */
    bool interact_with_bomb(uint16_t id);

    /* Comprar arma */
    bool shop_weapon(WeaponModel weapon, uint16_t id);

    /* Comprar munición */
    bool shop_ammo(uint16_t ammo_amount, WeaponType weapon, uint16_t id);

    /* Cambiar arma */
    bool change_weapon(uint16_t id);

    /* Tomar arma */
    void pick_up_weapon(uint16_t id);

    /* Rotar */
    void rotate(float angle, uint16_t id);

    /* Salir */
    void quit(uint16_t id);

    /* Setters */
    void set_ready_to_start();

    /* Getters */
    std::vector<std::shared_ptr<Object>> get_objects();
    Stats get_stats();
};

#endif  // MONITOR_GAME_H
