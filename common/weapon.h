#ifndef WEAPON_H
#define WEAPON_H

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
    uint16_t ammo;

    void fix_height();

public:
    /* Constructor */
    Weapon(uint16_t id, WeaponModel model, uint16_t range, uint16_t _min_damage,
           uint16_t _max_damage, float _precision, uint16_t ammo = 30);

    Weapon();

    explicit Weapon(const WeaponDTO& weapon_dto);

    /* Verificaciones */
    bool is_bomb();

    /* Getters */
    uint16_t get_range();

    std::vector<uint16_t> get_damage();

    WeaponDTO get_weapon_dto() const;

    ObjectDTO get_dto() const override;

    WeaponModel get_model() const;

    uint16_t get_ammo() const;

    /* Setters */
    void set_position(const std::vector<uint16_t>& new_position) { position = new_position; }

    bool shoot();

    /* Funcionalidades */
    void add_ammo(uint16_t ammo_amount);

    bool operator==(const Weapon& other) const;

    std::string get_name() const {
        switch (model) {
            case WeaponModel::KNIFE:
                return "Knife";
            case WeaponModel::GLOCK:
                return "Glock";
            case WeaponModel::AK47:
                return "AK-47";
            case WeaponModel::M3:
                return "M3";
            case WeaponModel::AWP:
                return "AWP";
            case WeaponModel::BOMB:
                return "Bomb";
            default:
                return "Unknown Weapon";
        }
    }
};

#endif  // WEAPON_H
