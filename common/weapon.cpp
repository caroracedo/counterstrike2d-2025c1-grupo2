#include "weapon.h"

#include <iostream>

/* Constructor */
Weapon::Weapon(uint16_t id, WeaponModel model, uint16_t range, uint16_t _min_damage,
               uint16_t _max_damage, float _precision, float _cooldown, uint16_t ammo):
        Object(ObjectType::WEAPON, id, {0, 0}, WEAPON_WIDTH, WEAPON_WIDTH),
        model(model),
        range(range),
        min_damage(_min_damage),
        max_damage(_max_damage),
        precision(_precision),
        cooldown(_cooldown),
        ammo(ammo),
        infinite_ammo(false) {
    fix_height();
    last_shot_time = std::chrono::steady_clock::now() -
                     std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                             std::chrono::duration<float>(cooldown));
}

Weapon::Weapon():
        Object(ObjectType::WEAPON, 0, {0, 0}, WEAPON_WIDTH, WEAPON_WIDTH),
        model(WeaponModel::UNKNOWN),
        range(0),
        min_damage(0),
        max_damage(0),
        precision(0.0f),
        cooldown(0.0f),
        ammo(0) {
    fix_height();
    last_shot_time = std::chrono::steady_clock::now() -
                     std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                             std::chrono::duration<float>(cooldown));
}

Weapon::Weapon(const WeaponDTO& weapon_dto):
        Object(ObjectType::WEAPON, weapon_dto.id, {0, 0}, WEAPON_WIDTH, WEAPON_WIDTH),
        model(weapon_dto.model),
        range(weapon_dto.range),
        min_damage(weapon_dto.min_damage),
        max_damage(weapon_dto.max_damage),
        precision(weapon_dto.precision),
        cooldown(weapon_dto.cooldown),
        ammo(weapon_dto.ammo) {
    fix_height();
    last_shot_time = std::chrono::steady_clock::now() -
                     std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                             std::chrono::duration<float>(cooldown));
}


ObjectDTO Weapon::get_dto() const { return ObjectDTO(ObjectType::WEAPON, position, model); }

WeaponDTO Weapon::get_weapon_dto() const {
    return WeaponDTO(id, model, range, min_damage, max_damage, precision, cooldown, ammo);
}

bool Weapon::is_bomb() { return model == WeaponModel::BOMB; }

bool Weapon::shoot() {
    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float>(now - last_shot_time).count();
    if (elapsed < cooldown) {
        return false;  // No se puede disparar aún, cooldown no cumplido
    }
    last_shot_time = now;  // Actualizar el tiempo del último disparo
    if (infinite_ammo) {
        return true;  // Disparo exitoso sin gastar munición
    }
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

uint16_t Weapon::get_range() { return range; }

std::vector<uint16_t> Weapon::get_damage() { return {min_damage, max_damage}; }

WeaponModel Weapon::get_model() const { return model; }

std::string Weapon::get_name() const {
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

uint16_t Weapon::get_ammo() const { return ammo; }

void Weapon::add_ammo(uint16_t ammo_amount) { ammo += ammo_amount; }

void Weapon::set_infinite_ammo() {
    infinite_ammo = true;
    ammo = 9999;
}

void Weapon::set_position(const std::vector<uint16_t>& new_position) { position = new_position; }

bool Weapon::operator==(const Weapon& other) const { return this->model == other.model; }

void Weapon::fix_height() {
    switch (model) {
        case WeaponModel::AK47: {
            height = AK47_HEIGHT;
            break;
        }
        case WeaponModel::M3: {
            height = M3_HEIGHT;
            break;
        }
        case WeaponModel::AWP: {
            height = AWP_HEIGHT;
            break;
        }
        default:
            break;
    }
}
