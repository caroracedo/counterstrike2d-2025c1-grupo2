#ifndef WEAPON_SHOP_H
#define WEAPON_SHOP_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "config.h"
#include "weapon.h"

#define AMMO_PRICE 1  // TODO: Agregar al config...

class WeaponShop {
private:
    std::unordered_map<WeaponModel, WeaponConfig> catalog;

public:
    explicit WeaponShop(Config& config): catalog(config.get_weapon_config()) {}

    std::pair<uint16_t, Weapon> buy_weapon(const WeaponModel& weapon_model, uint16_t player_money) {
        auto it = catalog.find(weapon_model);
        if (it != catalog.end() && player_money >= it->second.price) {
            return {it->second.price, Weapon(weapon_model, it->second.range, it->second.damage)};
        }
        return {0, Weapon()};
    }

    std::pair<uint16_t, bool> buy_ammo(uint16_t ammo_amount, uint16_t player_money) {
        if (player_money >= ammo_amount * AMMO_PRICE) {
            return {ammo_amount * AMMO_PRICE, true};
        }
        return {0, false};
    }
};

#endif  // WEAPON_SHOP_H
