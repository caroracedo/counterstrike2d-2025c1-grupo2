#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>
#include <memory>
#include <vector>

#include "bullet.h"
#include "types.h"

// Por ahora no es técnicamente un objeto, capaz si cuando tenga que estar en el piso y eso en el
// lobby
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
            range(5) {}  // Por ahora hardcodeado, dependerá del tipo de arma

    /* Verificaciones*/
    bool is_bomb() { return model == WeaponModel::BOMB; }

    /* Funcionalidades */
    /* Disparar */
    std::shared_ptr<Bullet> shoot(std::vector<uint16_t> shoot_position,
                                  std::vector<uint16_t> desired_position) {
        return std::make_shared<Bullet>(shoot_position, range, 5,
                                        desired_position);  // Por ahora el daño es 5, después se
                                                            // calculará según el tipo de arma
    }

    /* Getters */
    uint16_t get_price() { return price; }
};

#endif  // WEAPON_H
