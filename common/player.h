#ifndef PLAYER_H
#define PLAYER_H

#include <utility>
#include <vector>

#include "object.h"
#include "weapon.h"
#include "weapon_shop.h"

#define PLAYER_SIZE 32

class Player: public Object {
private:
    PlayerType player_type;
    uint16_t health;
    uint16_t money;
    uint16_t radius = PLAYER_SIZE / 2;

    WeaponShop& weapon_shop;

    Weapon knife;
    Weapon primary_weapon;
    Weapon secondary_weapon;
    Weapon bomb;

    Weapon current;

public:
    /* Constructor */
    Player(uint16_t id, const std::vector<uint16_t>& position, PlayerType type, uint8_t health,
           uint16_t initial_money, WeaponShop& weapon_shop, bool has_bomb):
            Object(ObjectType::PLAYER, id, position, PLAYER_SIZE, PLAYER_SIZE),
            player_type(type),
            health(health),
            money(initial_money),
            weapon_shop(weapon_shop) {
        std::pair<uint16_t, Weapon> new_knife = weapon_shop.buy_weapon(WeaponModel::KNIFE, money);
        money -= new_knife.first;
        knife = new_knife.second;

        std::pair<uint16_t, Weapon> new_secondary_weapon =
                weapon_shop.buy_weapon(WeaponModel::GLOCK, money);
        money -= new_secondary_weapon.first;
        secondary_weapon = new_secondary_weapon.second;

        if (has_bomb) {
            std::pair<uint16_t, Weapon> new_bomb = weapon_shop.buy_weapon(WeaponModel::BOMB, money);
            money -= new_bomb.first;
            bomb = new_bomb.second;
        }

        current = secondary_weapon;
    }

    /* Virtual puro */
    /* Getters */
    ObjectDTO get_dto() const override {
        return ObjectDTO(object_type, position, id, player_type, current.get_model(), health,
                         money);
    }

    /* Verificaciones */
    bool is_alive() const { return health > 0; }

    /* Getters */
    Weapon get_current_weapon() const { return current; }

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
    void change_weapon() {
        if (current == primary_weapon) {
            current = secondary_weapon;
        } else if (current == secondary_weapon) {
            if (bomb.is_bomb()) {
                current = bomb;
            } else {
                current = knife;
            }
        } else if (current == bomb) {
            current = knife;
        } else if (current == knife) {
            current = primary_weapon;
        }
    }

    std::vector<uint16_t> get_next_position(Direction direction) const {
        uint16_t min_position = radius;
        uint16_t max_position = MATRIX_SIZE * CELL_SIZE - radius;
        std::vector<uint16_t> new_position = position;
        switch (direction) {
            case Direction::UP:
                new_position[1] = (position[1] > min_position + MOVE_STEP) ?
                                          position[1] - MOVE_STEP :
                                          min_position;
                break;
            case Direction::DOWN:
                new_position[1] = (position[1] + MOVE_STEP < max_position) ?
                                          position[1] + MOVE_STEP :
                                          max_position;
                break;
            case Direction::LEFT:
                new_position[0] = (position[0] > min_position + MOVE_STEP) ?
                                          position[0] - MOVE_STEP :
                                          min_position;
                break;
            case Direction::RIGHT:
                new_position[0] = (position[0] + MOVE_STEP < max_position) ?
                                          position[0] + MOVE_STEP :
                                          max_position;
                break;
            default:
                break;
        }
        return new_position;
    }

    /* Compra de arma */
    // Igual esto sería otro comando...
    bool buy_weapon(const WeaponModel& weapon_model) {
        std::pair<uint16_t, Weapon> purchase = weapon_shop.buy_weapon(weapon_model, money);
        if (purchase.second.get_model() == WeaponModel::UNKNOWN) {
            return false;
        }
        money -= purchase.first;
        primary_weapon = purchase.second;
        return true;
    }
};

#endif  // PLAYER
