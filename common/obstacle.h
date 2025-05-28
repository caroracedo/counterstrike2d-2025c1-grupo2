#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>

#include "object.h"

class Obstacle: public Object {
public:
    Obstacle(const std::vector<uint16_t>& position, uint16_t width, uint16_t height):
            Object(ObjectType::OBSTACLE, 0, position, width, height) {}
};

#endif  // OBSTACLE_H
