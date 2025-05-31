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

        target_position = {static_cast<uint16_t>(tx - BULLET_SIZE / 2.0f),
                           static_cast<uint16_t>(ty - BULLET_SIZE / 2.0f)};
    }
    std::vector<uint16_t> get_next_position() const {
        if (target_position.size() != 2)
            return position;

        // Centro actual de la bala
        float cx = position[0] + BULLET_SIZE / 2.0f;
        float cy = position[1] + BULLET_SIZE / 2.0f;

        // Vector hacia el centro objetivo
        int dx = static_cast<int>(target_position[0]) - static_cast<int>(cx);
        int dy = static_cast<int>(target_position[1]) - static_cast<int>(cy);

        float magnitude = std::sqrt(dx * dx + dy * dy);
        if (magnitude == 0) {
            return position;
        }

        float v_step = std::min(5.0f, static_cast<float>(range)) / magnitude;
        float step_dx = dx * v_step;
        float step_dy = dy * v_step;

        // Nuevo centro
        float nx_c = cx + step_dx;
        float ny_c = cy + step_dy;

        // Esquina superior izquierda real
        int nx = static_cast<int>(std::round(nx_c - BULLET_SIZE / 2.0f));
        int ny = static_cast<int>(std::round(ny_c - BULLET_SIZE / 2.0f));

        int max_pos = MATRIX_SIZE * CELL_SIZE - BULLET_SIZE;
        nx = std::max(0, std::min(nx, max_pos));
        ny = std::max(0, std::min(ny, max_pos));
        return {static_cast<uint16_t>(nx), static_cast<uint16_t>(ny)};
    }
    bool is_alive() const {
        float cx = position[0] + BULLET_SIZE / 2.0f;
        float cy = position[1] + BULLET_SIZE / 2.0f;
        return range > 0 && (static_cast<int>(cx) != static_cast<int>(target_position[0]) ||
                             static_cast<int>(cy) != static_cast<int>(target_position[1]));
    }

    /* Funcionalidades */
    void decrement_range(uint16_t distance_moved) { range -= distance_moved; }
};

#endif  // BULLET_H
