#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>
#include <memory>
#include <vector>

#include "bullet.h"
#include "types.h"

class Weapon {
private:
    WeaponModel model;
    uint16_t range;
    uint16_t damage;
    uint16_t ammo;

public:
    /* Constructor */
    Weapon(WeaponModel model, uint16_t range, u_int16_t damage, uint16_t ammo = 30):
            model(model), range(range), damage(damage), ammo(ammo) {}

    Weapon(): model(WeaponModel::UNKNOWN), range(), damage(), ammo() {}

    /* Verificaciones */
    bool is_bomb() { return model == WeaponModel::BOMB; }

    /* Getters */
    uint16_t get_range() { return range; }
    uint16_t get_damage() {
        return damage;
    }  // El daño depende de la distancia, se podría pasar y calcularlo desde ahí, la consigna dice
       // que es inversamente proporcional...
    WeaponModel get_model() const { return model; }
    uint16_t get_ammo() const { return ammo; }

    /* Funcionalidades */
    void add_ammo(uint16_t ammo_amount) { ammo += ammo_amount; }

    bool operator==(const Weapon& other) const {
        // Compara los atributos relevantes, por ejemplo el modelo o id
        return this->model == other.model;  // Ajusta según tu implementación
    }
};

#endif  // WEAPON_H
