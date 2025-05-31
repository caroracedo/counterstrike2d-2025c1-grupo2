#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>

#include "object.h"

class Obstacle: public Object {
public:
    Obstacle(const std::vector<uint16_t>& position, uint16_t width, uint16_t height):
            Object(ObjectType::OBSTACLE, 0, position, width, height) {}

    /* Virtual puro */
    /* Getters */
    ObjectDTO get_dto() const override { return ObjectDTO(object_type, position, width, height); }
};

#endif  // OBSTACLE_H
