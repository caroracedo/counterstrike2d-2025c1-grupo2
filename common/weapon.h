#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>
#include <vector>

#include "object.h"

class Weapon: public Object {
protected:
    uint16_t price;
    uint16_t range;

public:
    Weapon(uint16_t price, uint16_t range, const std::vector<uint16_t>& position = {0, 0}):
            Object(ObjectType::WEAPON, -1, 8, 8, position),
            price(price),
            range(range) {}  // Tamaño del arma (8x8)

    // Aquí puedes agregar métodos específicos para el arma si es necesario
};

#endif  // WEAPON_H
