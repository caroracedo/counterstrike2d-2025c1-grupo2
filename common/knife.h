#ifndef KNIFE_H
#define KNIFE_H

#include <algorithm>
#include <random>
#include <vector>

#include "constants.h"
#include "object.h"
#include "object_DTO.h"

class Knife: public Object {
    uint16_t range;
    uint16_t min_damage;
    uint16_t max_damage;
    std::vector<uint16_t> start_position;
    std::vector<uint16_t> end_position;

public:
    Knife(const std::vector<uint16_t>& player_position, uint16_t _range, uint16_t _min_damage,
          uint16_t _max_damage, const std::vector<uint16_t>& desired_position):
            Object(ObjectType::KNIFE, 0, player_position, 0, 0),
            range(_range),
            min_damage(_min_damage),
            max_damage(_max_damage),
            start_position(player_position) {
        // Calcular punto final del ataque
        float dx = static_cast<float>(desired_position[0]) - static_cast<float>(player_position[0]);
        float dy = static_cast<float>(desired_position[1]) - static_cast<float>(player_position[1]);
        float magnitude = std::sqrt(dx * dx + dy * dy);
        float ux = (magnitude == 0) ? 0 : dx / magnitude;
        float uy = (magnitude == 0) ? 0 : dy / magnitude;
        end_position = {static_cast<uint16_t>(player_position[0] + ux * range),
                        static_cast<uint16_t>(player_position[1] + uy * range)};
    }

    uint16_t get_damage() const {
        // Devuelve el daño aleatorio del cuchillo
        static thread_local std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<uint16_t> dmg_dist(min_damage, max_damage);
        return dmg_dist(rng);
    }

    bool hits(const std::vector<uint16_t>& target_pos) const {
        // Verifica si el objetivo está en el rango del cuchillo
        float px = static_cast<float>(target_pos[0]);
        float py = static_cast<float>(target_pos[1]);
        float vx = end_position[0] - start_position[0];
        float vy = end_position[1] - start_position[1];
        float wx = px - start_position[0];
        float wy = py - start_position[1];

        float c1 = vx * wx + vy * wy;
        float c2 = vx * vx + vy * vy;
        float b = (c2 == 0) ? 0 : c1 / c2;
        b = std::clamp(b, 0.0f, 1.0f);

        float closest_x = start_position[0] + b * vx;
        float closest_y = start_position[1] + b * vy;
        float dist = std::sqrt((px - closest_x) * (px - closest_x) +
                               (py - closest_y) * (py - closest_y));
        float dist_to_attacker =
                std::sqrt((closest_x - start_position[0]) * (closest_x - start_position[0]) +
                          (closest_y - start_position[1]) * (closest_y - start_position[1]));
        float player_to_target = std::sqrt((px - start_position[0]) * (px - start_position[0]) +
                                           (py - start_position[1]) * (py - start_position[1]));

        return (dist <= PLAYER_RADIUS && dist_to_attacker <= range && player_to_target <= range);
    }

    ObjectDTO get_dto() const override { return {ObjectType::KNIFE, start_position}; }
};

#endif  // KNIFE_H
