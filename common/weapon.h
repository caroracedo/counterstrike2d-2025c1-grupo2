#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>
#include <memory>
#include <vector>

#include "bullet.h"
#include "types.h"

// TODO: Por ahora no es técnicamente un objeto, capaz si cuando tenga que estar en el piso y eso en
// el lobby
class Weapon {
private:
    WeaponModel model;
    uint16_t price;
    uint16_t range;

public:
    /* Constructor */
    explicit Weapon(WeaponModel model):
            model(model),
            price(30),
            range(5) {}  // TODO: Por ahora hardcodeado, dependerá del tipo de arma

    /* Verificaciones*/
    bool is_bomb() { return model == WeaponModel::BOMB; }

    /* Funcionalidades */
    /* Disparar */
    // std::shared_ptr<Bullet> shoot(std::vector<uint16_t> shoot_position,
    //                               std::vector<uint16_t> desired_position) {
    //     return std::make_shared<Bullet>(shoot_position, range, 5,
    //                                     desired_position);  // Por ahora el daño es 5, después se
    //                                                         // calculará según el tipo de arma
    // }

    /* Getters */
    uint16_t get_price() { return price; }
    uint16_t get_range() { return range; }
    uint16_t get_damage() {
        switch (model) {
            case WeaponModel::KNIFE:
                return 50;  // Daño del cuchillo
            case WeaponModel::GLOCK:
                return 20;  // Daño de la Glock
            case WeaponModel::AK47:
                return 30;  // Daño del AK47
            case WeaponModel::M3:
                return 40;  // Daño de la M3
            case WeaponModel::AWP:
                return 100;  // Daño del AWP
            case WeaponModel::BOMB:
                return 200;  // Daño de la bomba
            default:
                return 0;  // Daño desconocido
        }
    }
    WeaponModel get_model() const { return model; }
};

#endif  // WEAPON_H
