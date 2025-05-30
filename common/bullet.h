#ifndef BULLET_H
#define BULLET_H

#include <algorithm>
#include <cstdint>
#include <vector>

#include "object.h"

class Bullet: public Object {
private:
    uint16_t range;
    uint16_t damage;
    std::vector<uint16_t> target_position;

public:
    /* Constructor */
    Bullet(const uint16_t id, const std::vector<uint16_t>& position, uint16_t range,
           uint16_t damage):
            Object(ObjectType::BULLET, id, position, BULLET_SIZE, BULLET_SIZE),
            range(range),
            damage(damage),
            target_position(position) {}

    /* Getters */
    uint16_t get_range() const { return range; }

    uint16_t get_damage() const { return damage; }

    void set_target_position(const std::vector<uint16_t>& desired_position) {
        if (desired_position.size() != 2)
            return;

        int dx = static_cast<int>(desired_position[0]) - static_cast<int>(position[0]);
        int dy = static_cast<int>(desired_position[1]) - static_cast<int>(position[1]);

        float magnitude = std::sqrt(dx * dx + dy * dy);
        if (magnitude == 0) {
            target_position = position;
            return;
        }

        float scale_factor = static_cast<float>(range) / magnitude;
        int true_dx = static_cast<int>(dx * scale_factor);
        int true_dy = static_cast<int>(dy * scale_factor);

        int max_pos = MATRIX_SIZE * CELL_SIZE - BULLET_SIZE;
        int tx = static_cast<int>(position[0]) + true_dx;
        int ty = static_cast<int>(position[1]) + true_dy;

        tx = std::max(0, std::min(tx, max_pos));
        ty = std::max(0, std::min(ty, max_pos));

        target_position = {static_cast<uint16_t>(tx), static_cast<uint16_t>(ty)};
    }

    std::vector<uint16_t> get_next_position() const {
        if (target_position.size() != 2)
            return position;

        int dx = static_cast<int>(target_position[0]) - static_cast<int>(position[0]);
        int dy = static_cast<int>(target_position[1]) - static_cast<int>(position[1]);

        float magnitude = std::sqrt(dx * dx + dy * dy);
        if (magnitude == 0) {
            return position;
        }

        float v_step = 5.0f / magnitude;  // 5 unidades por paso
        int step_dx = static_cast<int>(dx * v_step);
        int step_dy = static_cast<int>(dy * v_step);

        int max_pos = MATRIX_SIZE * CELL_SIZE - BULLET_SIZE;
        int nx = static_cast<int>(position[0]) + step_dx;
        int ny = static_cast<int>(position[1]) + step_dy;

        nx = std::max(0, std::min(nx, max_pos));
        ny = std::max(0, std::min(ny, max_pos));
        return {static_cast<uint16_t>(nx), static_cast<uint16_t>(ny)};
    }

    bool is_alive() const { return range > 5; }

    /* Funcionalidades */
    void decrement_range() {
        if (range > 0) {
            range -= 5;  // Decrementa el rango de la bala en 5 unidades
        }
    }
};

#endif  // BULLET_H
