#ifndef WEAPON_H
#define WEAPON_H

#include "object.h"
#include <cstdint>
#include <vector>

class Weapon : public Object {
protected:
    uint16_t price;
    uint16_t range;

public:
    Weapon(uint16_t price, uint16_t range, const std::vector<uint16_t>& position = {0, 0})
        : Object(ObjectType::WEAPON, -1, 8, 8, position), price(price), range(range) {}  // Tamaño del arma (10x10)

    // Aquí puedes agregar métodos específicos para el arma si es necesario
};

#endif  // WEAPON_H