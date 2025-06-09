#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "object.h"
#include "weapon.h"
#include "weapon_DTO.h"
#include "weapon_shop.h"

class Player: public Object {
private:
    PlayerType player_type;
    uint16_t health;
    uint16_t money;
    uint16_t radius = PLAYER_RADIUS;

    WeaponShop& weapon_shop;

    Weapon knife;
    Weapon primary_weapon;
    Weapon secondary_weapon;
    bool has_bomb = false;

    Weapon current;

public:
    /* Constructor */
    Player(uint16_t id, const std::vector<uint16_t>& position, PlayerType type, uint8_t health,
           uint16_t initial_money, WeaponShop& weapon_shop);

    /* Virtual puro */
    ObjectDTO get_dto() const override;

    /* Verificaciones */
    bool is_alive() const;

    /* Getters */
    WeaponDTO get_current_weapon() const;

    bool shoot();

    PlayerType get_player_type() const;

    /* Funcionalidades */
    /* Daño */
    void take_damage(uint16_t damage);

    void cure(uint16_t health_amount);

    void switch_player_type();

    /* Cambio de arma */
    void change_weapon();

    bool can_plant_bomb() const;

    void plant_bomb();

    /* Setters */
    void set_bomb();

    std::string get_current_weapon_name() const;

    std::vector<uint16_t> get_next_position(Direction direction) const;

    /* Comprar Weapon */
    bool buy_weapon(const WeaponModel& weapon_model);

    /* Comprar Ammo */
    bool buy_ammo(WeaponType weapon, uint16_t ammo_amount);

    void add_money(uint16_t amount);
};

#endif  // PLAYER
