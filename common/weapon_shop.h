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
    uint16_t next_weapon_id = 0;

public:
    /* Constructor */
    explicit WeaponShop(Config& config): catalog(config.get_weapon_config()) {}

    std::pair<uint16_t, Weapon> sell_weapon(const WeaponModel& weapon_model, uint16_t player_money);

    Weapon give_weapon(const WeaponModel& weapon_model);

    std::pair<uint16_t, bool> sell_ammo(uint16_t ammo_amount, uint16_t player_money);
};

#endif  // WEAPON_SHOP_H
