#ifndef WEAPON_SHOP_H
#define WEAPON_SHOP_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "weapon.h"

#define KNIFE_PRICE 0
#define GLOCK_PRICE 0
#define BOMB_PRICE 0
#define AK47_PRICE 100
#define M3_PRICE 200
#define AWP_PRICE 300

#define KNIFE_RANGE 1
#define GLOCK_RANGE 50
#define BOMB_RANGE 0
#define AK47_RANGE 100
#define M3_RANGE 50
#define AWP_RANGE 150

#define KNIFE_DAMAGE 10
#define GLOCK_DAMAGE 20
#define BOMB_DAMAGE 500
#define AK47_DAMAGE 10
#define M3_DAMAGE 30
#define AWP_DAMAGE 40

struct WeaponItem {
    uint16_t price;
    uint16_t range;
    uint16_t damage;
};

class WeaponShop {
private:
    std::unordered_map<WeaponModel, WeaponItem> catalog;

    void initialize_catalog() {
        catalog[WeaponModel::KNIFE] = {KNIFE_PRICE, KNIFE_RANGE, KNIFE_DAMAGE};
        catalog[WeaponModel::GLOCK] = {GLOCK_PRICE, GLOCK_RANGE, GLOCK_DAMAGE};
        catalog[WeaponModel::BOMB] = {BOMB_PRICE, BOMB_RANGE, BOMB_DAMAGE};
        catalog[WeaponModel::AK47] = {AK47_PRICE, AK47_RANGE, AK47_DAMAGE};
        catalog[WeaponModel::M3] = {M3_PRICE, M3_RANGE, M3_DAMAGE};
        catalog[WeaponModel::AWP] = {AWP_PRICE, AWP_RANGE, AWP_DAMAGE};
    }

public:
    WeaponShop() { initialize_catalog(); }

    std::pair<uint16_t, Weapon> buy_weapon(const WeaponModel& weapon_model, uint16_t player_money) {
        auto it = catalog.find(weapon_model);
        if (it != catalog.end() && player_money >= it->second.price) {
            return {it->second.price, Weapon(weapon_model, it->second.range, it->second.damage)};
        }
        return {0, Weapon()};
    }
};

#endif  // WEAPON_SHOP_H
