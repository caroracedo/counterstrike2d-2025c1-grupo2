#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "object.h"
#include "weapon.h"

#define PLAYER_SIZE 32

class Player: public Object {
private:
    PlayerType player_type;
    Weapon primary_weapon;
    Weapon secondary_weapon;
    Weapon knife;
    Weapon bomb;

    Weapon current;

    uint16_t health;
    uint16_t money;

public:
    Player(uint16_t id, const std::vector<uint16_t>& position, PlayerType player_type,
           bool has_bomb):
            Object(ObjectType::PLAYER, id, position, PLAYER_SIZE, PLAYER_SIZE),
            player_type(player_type),
            secondary_weapon(WeaponModel::GLOCK),
            knife(WeaponModel::KNIFE),
            has_bomb ? bomb(WeaponModel::Bomb) : bomb(),
            current(secondary_weapon),
            health(100),   // Por defecto, el jugador comienza con 100 de salud
            money(500) {}  // Por defecto, el jugador comienza con 500 de dinero

    /* Virtual puro */
    /* Verificaciones */
    bool is_alive() const override { return health > 0; }

    /* Funcionalidades */
    /* Daño */
    void take_damage(uint16_t damage) {
        if (health > damage) {
            health -= damage;
        } else {
            health = 0;  // El jugador muere
        }
    }

    /* Cambio de arma */
    bool change_weapon(const WeaponType& weapon_type) {
        if (weapon_type == WeaponType::PRIMARY) {
            current = primary_weapon;
        } else if (weapon_type == WeaponType::SECONDARY) {
            current = secondary_weapon;
        } else if (weapon_type == WeaponType::KNIFE) {
            current = knife;
        } else if (weapon_type == WeaponType::BOMB &&
                   bomb.is_bomb()) {  // En realidad estoy checkeando si no es UNKNOWN
            current = bomb;
        } else {
            return false;
        }
        return true;
    }

    /* Compra de arma */
    // Igual esto sería otro comando...
    bool buy_weapon(const WeaponModel& weapon_model) {
        Weapon new_weapon = Weapon(weapon_model);
        if (new_weapon.get_price() > money)
            return false;
        primary_weapon = new_weapon;
        return true;
    }
};

#endif  // PLAYER
