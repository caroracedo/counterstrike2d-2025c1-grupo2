#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>

#include "object.h"

class Obstacle: public Object {
private:
    ObstacleType obstacle_type;

public:
    Obstacle(const std::vector<uint16_t>& position, uint16_t width, uint16_t height,
             ObstacleType obstacle_type):
            Object(ObjectType::OBSTACLE, 0, position, width, height),
            obstacle_type(obstacle_type) {}

    /* Virtual puro */
    /* Getters */
    ObjectDTO get_dto() const override {
        return ObjectDTO(object_type, position, width, height, obstacle_type);
    }
};

#endif  // OBSTACLE_H
