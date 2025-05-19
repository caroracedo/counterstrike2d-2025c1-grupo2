#ifndef WEAPON_DTO_H
#define WEAPON_DTO_H

#include <cstdint>

enum class WeaponType : uint8_t { NONE = 0x00, GLOCK = 0x01, AK47 = 0x02, M3 = 0x03, AWP = 0x04 };

struct WeaponDTO {
    WeaponType type;
    uint16_t damage;
    uint16_t range;
    uint16_t ammo;

    // Constructor para armas
    WeaponDTO(WeaponType type, uint16_t damage, uint16_t range, uint16_t ammo):
            type(type), damage(damage), range(range), ammo(ammo) {}

    // Constructor para armas sin munición
    explicit WeaponDTO(WeaponType type): type(type), damage(0), range(50), ammo(30) {}

    // Constructor para armas sin munición y sin daño
    WeaponDTO(): type(WeaponType::NONE), damage(0), range(0), ammo(0) {}
};

#endif  // WEAPON_DTO_H
