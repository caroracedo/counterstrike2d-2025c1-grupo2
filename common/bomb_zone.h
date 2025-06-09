#ifndef BOMB_ZONE_H
#define BOMB_ZONE_H

#include <vector>

#include "object.h"
#include "object_DTO.h"

class BombZone: public Object {
public:
    BombZone(const std::vector<uint16_t>& position, uint16_t width, uint16_t height);

    ObjectDTO get_dto() const override;
};

#endif  // BOMB_ZONE_H
