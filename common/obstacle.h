#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>

#include "object.h"

class Obstacle: public Object {
public:
    Obstacle(uint16_t width, uint16_t height, const std::vector<uint16_t>& position):
            Object(ObjectType::OBSTACLE, -1, width, height, position) {}
};

#endif  // OBSTACLE_H
