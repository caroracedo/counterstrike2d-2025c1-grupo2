#include "weapon_shop.h"

WeaponShop::WeaponShop(Config& config): catalog(config.get_weapon_config()) {}

std::pair<uint16_t, Weapon> WeaponShop::sell_weapon(const WeaponModel& weapon_model,
                                                    uint16_t player_money) {
    auto it = catalog.find(weapon_model);
    if (it != catalog.end() && player_money >= it->second.price) {
        return {it->second.price,
                Weapon(++next_weapon_id, weapon_model, it->second.range, it->second.min_damage,
                       it->second.max_damage, it->second.precision, it->second.cooldown)};
    }
    return {0, Weapon()};
}

Weapon WeaponShop::give_weapon(const WeaponModel& weapon_model) {
    auto it = catalog.find(weapon_model);
    if (it != catalog.end()) {
        return Weapon(++next_weapon_id, weapon_model, it->second.range, it->second.min_damage,
                      it->second.max_damage, it->second.precision, it->second.cooldown);
    }
    return Weapon();
}

std::pair<uint16_t, bool> WeaponShop::sell_ammo(uint16_t ammo_amount, uint16_t player_money) {
    if (player_money >= ammo_amount * AMMO_PRICE) {
        return {ammo_amount * AMMO_PRICE, true};
    }
    return {0, false};
}
