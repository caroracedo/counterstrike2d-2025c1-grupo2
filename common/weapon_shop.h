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

    std::pair<uint16_t, bool> buy_ammo(uint16_t ammo_amount, uint16_t player_money) {
        uint16_t price_per_ammo = 10;
        uint16_t total_price = ammo_amount * price_per_ammo;

        if (player_money >= total_price) {
            return {total_price, true};
        }
        return {0, false};
    }
};

#endif  // WEAPON_SHOP_H
