#ifndef WEAPON_H
#define WEAPON_H

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "bullet.h"
#include "object.h"
#include "types.h"
#include "weapon_DTO.h"

class Weapon: public Object {
private:
    WeaponModel model;
    uint16_t range;
    uint16_t min_damage;
    uint16_t max_damage;
    float precision;
    float cooldown;
    uint16_t ammo;

    std::chrono::steady_clock::time_point last_shot_time;
    bool infinite_ammo = false;

    void fix_height();

public:
    /* Constructor */
    Weapon(uint16_t id, WeaponModel model, uint16_t range, uint16_t _min_damage,
           uint16_t _max_damage, float _precision, float _cooldown, uint16_t ammo = 30);

    Weapon();

    explicit Weapon(const WeaponDTO& weapon_dto);

    ObjectDTO get_dto() const override;
    WeaponDTO get_weapon_dto() const;

    bool is_bomb();

    bool shoot();

    uint16_t get_range();
    std::vector<uint16_t> get_damage();
    WeaponModel get_model() const;
    std::string get_name() const;

    uint16_t get_ammo() const;
    void add_ammo(uint16_t ammo_amount);

    void set_position(const std::vector<uint16_t>& new_position);

    void set_infinite_ammo() { infinite_ammo = true; }

    bool operator==(const Weapon& other) const;
};

#endif  // WEAPON_H
