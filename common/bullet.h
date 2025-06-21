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
    uint16_t range;
    uint16_t min_damage;
    uint16_t max_damage;
    uint16_t distance_moved = 0;
    uint16_t radius = BULLET_RADIUS;
    std::vector<uint16_t> target_position;
    std::vector<uint16_t> start_position;
    float precision;

    void set_target_position(const std::vector<uint16_t>& desired_position,
                             const std::vector<uint16_t>& player_position);

    void set_starting_position(const std::vector<uint16_t>& player_center);

public:
    /* Constructor */
    Bullet(const uint16_t id, const uint16_t _player_id,
           const std::vector<uint16_t>& player_position, uint16_t _range, uint16_t _min_damage,
           uint16_t _max_damage, float _precision, const std::vector<uint16_t>& desired_position);

    ObjectDTO get_dto() const override;

    bool is_alive() const;

    uint16_t get_range() const;
    void decrement_range(uint16_t dist);

    uint16_t get_player_id() const;
    uint16_t get_damage() const;
    std::vector<uint16_t> get_next_position() const;
};

#endif  // BULLET_H