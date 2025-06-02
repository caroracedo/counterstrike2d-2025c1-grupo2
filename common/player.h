#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <utility>
#include <vector>

#include "object.h"
#include "weapon.h"
#include "weapon_shop.h"

// #define PLAYER_SIZE 32

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
    bool has_bomb;

    Weapon current;

public:
    /* Constructor */
    Player(uint16_t id, const std::vector<uint16_t>& position, PlayerType type, uint8_t health,
           uint16_t initial_money, WeaponShop& weapon_shop, bool has_bomb):
            Object(ObjectType::PLAYER, id, position, PLAYER_RADIUS * 2, PLAYER_RADIUS * 2),
            player_type(type),
            health(health),
            money(initial_money),
            weapon_shop(weapon_shop),
            has_bomb(has_bomb) {

        std::pair<uint16_t, Weapon> new_knife = weapon_shop.buy_weapon(WeaponModel::KNIFE, money);
        money -= new_knife.first;
        knife = new_knife.second;

        std::pair<uint16_t, Weapon> new_secondary_weapon =
                weapon_shop.buy_weapon(WeaponModel::GLOCK, money);
        money -= new_secondary_weapon.first;
        secondary_weapon = new_secondary_weapon.second;

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
    PlayerType get_player_type() const { return player_type; }

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
        // primary_weapon -> secondary_weapon -> knife -> primary_weapon
        if (current == primary_weapon) {
            current = secondary_weapon;
        } else if (current == secondary_weapon) {
            current = knife;
        } else if (current == knife && primary_weapon.get_model() != WeaponModel::UNKNOWN) {
            current = primary_weapon;
        } else {
            current = secondary_weapon;  // Si no hay primario, vuelve al secundario
        }
    }

    bool can_plant_bomb() const { return has_bomb; }

    void plant_bomb() { has_bomb = false; }

    std::string get_current_weapon_name() const {
        switch (current.get_model()) {
            case WeaponModel::KNIFE:
                return "Knife";
            case WeaponModel::GLOCK:
                return "Glock";
            case WeaponModel::AK47:
                return "AK-47";
            case WeaponModel::M3:
                return "M3";
            case WeaponModel::AWP:
                return "AWP";
            case WeaponModel::BOMB:
                return "Bomb";
            default:
                return "Unknown Weapon";
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
