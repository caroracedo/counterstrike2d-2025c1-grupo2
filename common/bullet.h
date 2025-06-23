#ifndef BULLET_H
#define BULLET_H

#include <algorithm>
#include <cstdint>
#include <vector>

#include "constants.h"
#include "object.h"
#include "object_DTO.h"

class Bullet: public Object {
private:
    uint16_t player_id;
    const WeaponModel weapon_model;
    uint16_t range;
    const uint16_t min_damage;
    const uint16_t max_damage;
    uint16_t distance_moved;
    const uint16_t radius;
    std::vector<uint16_t> target_position;
    std::vector<uint16_t> start_position;
    const float precision;

    void set_starting_position(float angle_degrees, const std::vector<uint16_t>& player_center);

    void set_target_position_from_angle(float angle, const std::vector<uint16_t>& player_position);

public:
    /* Constructor */
    Bullet(const uint16_t id, const uint16_t _player_id,
           const std::vector<uint16_t>& player_position, WeaponModel _weapon_model, uint16_t _range,
           uint16_t _min_damage, uint16_t _max_damage, float _precision, float angle);

    ObjectDTO get_dto() const override;

    bool is_alive() const;

    uint16_t get_range() const;
    void decrement_range(uint16_t dist);

    uint16_t get_player_id() const;
    uint16_t get_damage() const;
    std::vector<uint16_t> get_next_position() const;
};

#endif  // BULLET_H
