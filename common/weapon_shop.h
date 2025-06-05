#ifndef WEAPON_SHOP_H
#define WEAPON_SHOP_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "config.h"
#include "weapon.h"

class WeaponShop {
private:
    std::unordered_map<WeaponModel, WeaponConfig> catalog;

public:
    explicit WeaponShop(Config& config): catalog(config.get_weapon_config()) {}

    std::pair<uint16_t, Weapon> buy_weapon(const WeaponModel& weapon_model, uint16_t player_money) {
        auto it = catalog.find(weapon_model);
        if (it != catalog.end() && player_money >= it->second.price) {
            // algunos numeros hardcodeados para que me compile
            return {it->second.price, Weapon(weapon_model, it->second.range, it->second.min_damage,
                                             it->second.max_damage, it->second.precision, 30)};
        }
        return {0, Weapon()};
    }
};

#endif  // WEAPON_SHOP_H
