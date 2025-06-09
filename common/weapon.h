#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>
#include <memory>
#include <vector>

#include "bullet.h"
#include "types.h"
#include "weapon_DTO.h"

class Weapon {
private:
    WeaponModel model;
    uint16_t range;
    uint16_t min_damage;
    uint16_t max_damage;
    float precision;
    uint16_t ammo;

public:
    /* Constructor */
    Weapon(WeaponModel model, uint16_t range, uint16_t _min_damage, uint16_t _max_damage,
           float _precision, uint16_t ammo = 30);

    Weapon();

    /* Verificaciones */
    bool is_bomb();

    /* Getters */
    uint16_t get_range();

    std::vector<uint16_t> get_damage();

    WeaponDTO get_dto() const;

    WeaponModel get_model() const;

    uint16_t get_ammo() const;

    bool shoot();

    /* Funcionalidades */
    void add_ammo(uint16_t ammo_amount);

    bool operator==(const Weapon& other) const;
};

#endif  // WEAPON_H
