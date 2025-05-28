#ifndef BULLET_H
#define BULLET_H

#include <cstdint>
#include "object.h"

class Bullet : public Object {
private:
    uint16_t range;  // Daño que causa la bala
    std::vector<uint16_t> desired_position;  // Hacia donde quiere ir la bala
public:
    Bullet(){}

    void set_range(uint16_t new_range) override {
        range = new_range;
    }

    uint16_t get_range() const {
        return range;
    }
};

#endif  // BULLET_H
