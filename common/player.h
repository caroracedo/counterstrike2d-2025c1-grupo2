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
    Player(uint16_t id, const std::vector<uint16_t>& position, PlayerType type, bool has_bomb):
            Object(ObjectType::PLAYER, id, position, PLAYER_SIZE, PLAYER_SIZE),
            player_type(type),
            primary_weapon({WeaponModel::AK47}),  // Para que no me tire error
            secondary_weapon({WeaponModel::GLOCK}),
            knife({WeaponModel::KNIFE}),
            bomb(has_bomb ? Weapon(WeaponModel::BOMB) : Weapon(WeaponModel::UNKNOWN)),
            current(secondary_weapon),
            health(100),   // Por defecto, el jugador comienza con 100 de salud
            money(500) {}  // Por defecto, el jugador comienza con 500 de dinero

    /* Verificaciones */
    bool is_alive() const { return health > 0; }

    /* Getters */
    Weapon get_current_weapon() const { return current; }
    PlayerType get_player_type() const { return player_type; }
    WeaponModel get_current_weapon_model() const { return current.get_model(); }

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

    std::vector<uint16_t> get_next_position(Direction direction) const {
        uint16_t max_position = MATRIX_SIZE * CELL_SIZE - PLAYER_SIZE;
        std::vector<uint16_t> new_position = position;
        switch (direction) {
            case Direction::UP:
                new_position[1] = (position[1] > MOVE_STEP) ? position[1] - MOVE_STEP : 0;
                break;
            case Direction::DOWN:
                new_position[1] = (position[1] + MOVE_STEP > max_position) ?
                                          max_position :
                                          position[1] + MOVE_STEP;
                break;
            case Direction::LEFT:
                new_position[0] = (position[0] > MOVE_STEP) ? position[0] - MOVE_STEP : 0;
                break;
            case Direction::RIGHT:
                new_position[0] = (position[0] + MOVE_STEP > max_position) ?
                                          max_position :
                                          position[0] + MOVE_STEP;
                break;
            default:
                break;
        }
        return new_position;
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
