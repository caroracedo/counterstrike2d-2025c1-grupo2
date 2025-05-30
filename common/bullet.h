#ifndef BULLET_H
#define BULLET_H

#include <algorithm>
#include <cstdint>
#include <numeric>
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

        if (dx == 0 && dy == 0) {
            target_position = position;
            return;
        }

        // Cantidad de pasos de 5 unidades
        int steps = range / 5;

        // Normaliza la dirección usando el máximo común divisor
        int gcd = std::gcd(std::abs(dx), std::abs(dy));
        int step_dx = (gcd == 0) ? 0 : dx / gcd;
        int step_dy = (gcd == 0) ? 0 : dy / gcd;

        // Movimiento total
        int tx = static_cast<int>(position[0]) + step_dx * steps * 5;
        int ty = static_cast<int>(position[1]) + step_dy * steps * 5;

        int max_pos = MATRIX_SIZE * CELL_SIZE - BULLET_SIZE;
        tx = std::max(0, std::min(tx, max_pos));
        ty = std::max(0, std::min(ty, max_pos));

        target_position = {static_cast<uint16_t>(tx), static_cast<uint16_t>(ty)};
    }

    std::vector<uint16_t> get_next_position() const {
        if (position == target_position)
            return position;

        int dx = static_cast<int>(target_position[0]) - static_cast<int>(position[0]);
        int dy = static_cast<int>(target_position[1]) - static_cast<int>(position[1]);

        if (dx == 0 && dy == 0)
            return position;

        // Paso entero de 5 unidades en la dirección correcta
        int step_x = (dx == 0) ? 0 : (dx > 0 ? 5 : -5);
        int step_y = (dy == 0) ? 0 : (dy > 0 ? 5 : -5);

        // Si el siguiente paso sobrepasa el target, ir directo al target
        int next_x =
                (std::abs(dx) < 5) ? target_position[0] : static_cast<int>(position[0]) + step_x;
        int next_y =
                (std::abs(dy) < 5) ? target_position[1] : static_cast<int>(position[1]) + step_y;

        int max_pos = MATRIX_SIZE * CELL_SIZE - BULLET_SIZE;
        next_x = std::max(0, std::min(next_x, max_pos));
        next_y = std::max(0, std::min(next_y, max_pos));

        return {static_cast<uint16_t>(next_x), static_cast<uint16_t>(next_y)};
    }

    bool is_alive() const { return position != target_position; }

    /* Funcionalidades */
    void decrement_range() {
        if (range >= 5) {
            range -= 5;  // Decrementa el rango de la bala en 5 unidades
        }
    }
};

#endif  // BULLET_H
