#ifndef WEAPON_DTO_H
#define WEAPON_DTO_H

#include <cstdint>
#include <string>

#include "types.h"

struct WeaponDTO {
    uint16_t id;
    WeaponModel model;
    uint16_t range;
    uint16_t min_damage;
    uint16_t max_damage;
    float precision;
    float cooldown;
    uint16_t ammo;

    WeaponDTO(uint16_t _id, const WeaponModel& _model, uint16_t _range, uint16_t _min_damage,
              uint16_t _max_damage, float _precision, float _cooldown, uint16_t _ammo):
            id(_id),
            model(_model),
            range(_range),
            min_damage(_min_damage),
            max_damage(_max_damage),
            precision(_precision),
            cooldown(_cooldown),
            ammo(_ammo) {}
    WeaponDTO():
            id(0),
            model(WeaponModel::UNKNOWN),
            range(0),
            min_damage(0),
            max_damage(0),
            precision(0.0f),
            cooldown(0.0f),
            ammo(0) {}
};

#endif  // WEAPON_DTO_H
