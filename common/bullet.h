#ifndef BULLET_H
#define BULLET_H

#include <cstdint>
#include <vector>

#include "object.h"

#define BULLET_SIZE 5
class Bullet: public Object {
private:
    uint16_t range;
    uint16_t damage;
    std::vector<uint16_t> desired_position;

public:
    /* Constructor */
    Bullet(const std::vector<uint16_t>& position, uint16_t range, uint16_t damage,
           const std::vector<uint16_t>& desired_position):
            Object(ObjectType::BULLET, -1, position, BULLET_SIZE, BULLET_SIZE),
            range(range),
            damage(damage),
            desired_position(desired_position) {}

    /* Getters */
    uint16_t get_range() const { return range; }

    uint16_t get_damage() const { return damage; }

    /* Verificaciones */
    /* Virtual puro */
    bool is_alive() const override { return range > 0; }

    /* Funcionalidades */
    void decrement_range() {
        if (range > 0) {
            range--;
        }
    }
};

#endif  // BULLET_H
