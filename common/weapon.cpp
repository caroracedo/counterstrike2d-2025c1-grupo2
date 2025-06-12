#include "weapon.h"

/* Constructor */
Weapon::Weapon(uint16_t id, WeaponModel model, uint16_t range, uint16_t _min_damage,
               uint16_t _max_damage, float _precision, uint16_t ammo):
        Object(ObjectType::WEAPON, id, {0, 0}, WEAPON_SIZE, WEAPON_SIZE),
        model(model),
        range(range),
        min_damage(_min_damage),
        max_damage(_max_damage),
        precision(_precision),
        ammo(ammo) {}

Weapon::Weapon():
        Object(ObjectType::WEAPON, 0, {0, 0}, WEAPON_SIZE, WEAPON_SIZE),
        model(WeaponModel::UNKNOWN),
        range(0),
        min_damage(0),
        max_damage(0),
        precision(0),
        ammo(0) {}

Weapon::Weapon(const WeaponDTO& weapon_dto):
        Object(ObjectType::WEAPON, weapon_dto.id, {0, 0}, WEAPON_SIZE, WEAPON_SIZE),
        model(weapon_dto.model),
        range(weapon_dto.range),
        min_damage(weapon_dto.min_damage),
        max_damage(weapon_dto.max_damage),
        precision(weapon_dto.precision),
        ammo(weapon_dto.ammo) {}

/* Verificaciones */
bool Weapon::is_bomb() { return model == WeaponModel::BOMB; }

/* Getters */
uint16_t Weapon::get_range() { return range; }

std::vector<uint16_t> Weapon::get_damage() { return {min_damage, max_damage}; }

WeaponDTO Weapon::get_weapon_dto() const {
    return WeaponDTO(id, model, range, min_damage, max_damage, precision, ammo);
}

ObjectDTO Weapon::get_dto() const {
    return ObjectDTO(ObjectType::WEAPON, position, get_weapon_dto());
}

WeaponModel Weapon::get_model() const { return model; }

uint16_t Weapon::get_ammo() const { return ammo; }

bool Weapon::shoot() {
    switch (model) {
        case WeaponModel::BOMB:
        case WeaponModel::KNIFE:
            return true;
        case WeaponModel::GLOCK:
        case WeaponModel::AWP:
            if (ammo > 0) {
                ammo--;
                return true;  // Disparo exitoso
            }
            return false;  // Sin munición
        case WeaponModel::AK47:
        case WeaponModel::M3:
            if (ammo >= 3) {
                ammo -= 3;    // Disparo de ráfaga
                return true;  // Disparo exitoso
            }
            return false;  // Sin munición
        default:
            return false;  // Modelo desconocido
    }
}

/* Funcionalidades */
void Weapon::add_ammo(uint16_t ammo_amount) { ammo += ammo_amount; }

bool Weapon::operator==(const Weapon& other) const { return this->model == other.model; }
