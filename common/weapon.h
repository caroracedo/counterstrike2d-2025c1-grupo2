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
           float _precision, uint16_t ammo = 30):
            model(model),
            range(range),
            min_damage(_min_damage),
            max_damage(_max_damage),
            precision(_precision),
            ammo(ammo) {}

    Weapon():
            model(WeaponModel::UNKNOWN),
            range(0),
            min_damage(0),
            max_damage(0),
            precision(0),
            ammo(0) {}

    /* Verificaciones */
    bool is_bomb() { return model == WeaponModel::BOMB; }

    /* Getters */
    uint16_t get_range() { return range; }

    std::vector<uint16_t> get_damage() { return {min_damage, max_damage}; }

    WeaponDTO get_dto() const {
        return WeaponDTO(model, range, min_damage, max_damage, precision, ammo);
    }

    WeaponModel get_model() const { return model; }

    uint16_t get_ammo() const { return ammo; }

    bool shoot() {
        switch (model) {
            case WeaponModel::BOMB:
            case WeaponModel::KNIFE:
                return true;
            case WeaponModel::GLOCK:
            case WeaponModel::AWP:
                if (ammo > 0) {
                    std::cout << "\tCantidad de balas antes del disparo: " << ammo << std::endl;
                    ammo--;
                    std::cout << "\tCantidad de balas después del disparo: " << ammo << std::endl;
                    return true;  // Disparo exitoso
                }
                std::cout << "No hay munición suficiente para disparar." << std::endl;
                return false;  // Sin munición
            case WeaponModel::AK47:
            case WeaponModel::M3:
                if (ammo >= 3) {
                    std::cout << "\tCantidad de balas antes del disparo: " << ammo << std::endl;
                    ammo -= 3;  // Disparo de ráfaga
                    std::cout << "\tCantidad de balas después del disparo: " << ammo << std::endl;
                    return true;  // Disparo exitoso
                }
                std::cout << "No hay munición suficiente para disparar." << std::endl;
                return false;  // Sin munición
            default:
                return false;  // Modelo desconocido
        }
    }

    /* Funcionalidades */
    void add_ammo(uint16_t ammo_amount) { ammo += ammo_amount; }

    bool operator==(const Weapon& other) const {
        // Compara los atributos relevantes, por ejemplo el modelo o id
        return this->model == other.model;  // Ajusta según tu implementación
    }
};

#endif  // WEAPON_H
