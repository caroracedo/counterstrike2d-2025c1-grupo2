#ifndef KNIFE_H
#define KNIFE_H

#include <algorithm>
#include <random>
#include <vector>

#include "constants.h"
#include "object.h"
#include "object_DTO.h"

class Knife: public Object {
    uint16_t player_id;
    uint16_t range;
    uint16_t min_damage;
    uint16_t max_damage;
    std::vector<uint16_t> start_position;
    std::vector<uint16_t> end_position;

public:
    Knife(const std::vector<uint16_t>& player_position, const uint16_t player_id, uint16_t _range,
          uint16_t _min_damage, uint16_t _max_damage,
          const std::vector<uint16_t>& desired_position);

    ObjectDTO get_dto() const override;

    uint16_t get_damage() const;

    bool hits(const std::vector<uint16_t>& target_pos) const;
};

#endif  // KNIFE_H
