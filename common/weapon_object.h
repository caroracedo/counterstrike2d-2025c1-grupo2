#ifndef WEAPON_OBJECT_H
#define WEAPON_OBJECT_H

#include <cstdint>
#include <memory>
#include <vector>

#include "bullet.h"
#include "types.h"

#define WEAPON_SIZE 32

// Para diferenciar el arma que se porta del arma que se dropea...
// TODO: Ver si hay una mejor forma...
class WeaponObject: public Object {
private:
    WeaponModel model;
    uint16_t range;
    uint16_t damage;
    uint16_t ammo;

public:
    /* Constructor */
    WeaponObject(const std::vector<uint16_t>& position, WeaponModel model, uint16_t range,
                 u_int16_t damage, uint16_t ammo = 30):
            Object(ObjectType::WEAPON, 0, position, WEAPON_SIZE, WEAPON_SIZE),
            model(model),
            range(range),
            damage(damage),
            ammo(ammo) {}
};

#endif  // WEAPON_OBJECT_H
