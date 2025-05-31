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

    /* Virtual puro */
    /* Getters */
    ObjectDTO get_dto() const override { return ObjectDTO(object_type, position); }

    /* Getters */
    uint16_t get_range() const { return range; }

    uint16_t get_damage() const { return damage; }

    void set_target_position(const std::vector<uint16_t>& desired_position,
                             const std::vector<uint16_t>& player_position) {
        if (desired_position.size() != 2 || player_position.size() != 2)
            return;

        // Calcular el centro del jugador
        float cx = player_position[0] + PLAYER_SIZE / 2.0f;
        float cy = player_position[1] + PLAYER_SIZE / 2.0f;

        // Calcular el vector dirección desde el centro del jugador al destino
        float dx = static_cast<float>(desired_position[0]) - cx;
        float dy = static_cast<float>(desired_position[1]) - cy;
        float magnitude = std::sqrt(dx * dx + dy * dy);
        if (magnitude == 0) {
            target_position = position;
            return;
        }

        float scale_factor = static_cast<float>(range) / magnitude;
        int true_dx = static_cast<int>(dx * scale_factor);
        int true_dy = static_cast<int>(dy * scale_factor);

        int tx = static_cast<int>(cx) + true_dx;
        int ty = static_cast<int>(cy) + true_dy;

        int max_pos = MATRIX_SIZE * CELL_SIZE - BULLET_SIZE;
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

        float v_step =
                std::min(5.0f, static_cast<float>(range)) / magnitude;  // 5 unidades por paso
        int step_dx = static_cast<int>(dx * v_step);
        int step_dy = static_cast<int>(dy * v_step);

        int max_pos = MATRIX_SIZE * CELL_SIZE - BULLET_SIZE;
        int nx = static_cast<int>(position[0]) + step_dx;
        int ny = static_cast<int>(position[1]) + step_dy;

        nx = std::max(0, std::min(nx, max_pos));
        ny = std::max(0, std::min(ny, max_pos));
        return {static_cast<uint16_t>(nx), static_cast<uint16_t>(ny)};
    }

    bool is_alive() const {
        return range > 0 && position[0] != target_position[0] && position[1] != target_position[1];
    }

    /* Funcionalidades */
    void decrement_range(uint16_t distance_moved) { range -= distance_moved; }
};

#endif  // BULLET_H
