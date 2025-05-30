#ifndef ACTION_DTO_H
#define ACTION_DTO_H

#include <cstdint>
#include <vector>

#include "object_DTO.h"

enum class ActionType : uint8_t {
    MOVE = 0x6D,
    QUIT = 0x71,
    SHOOT = 0x73,
    UPDATE = 0x75,
    UNKNOWN = 0x00
};
enum class Direction : uint8_t {
    UP = 0x01,
    DOWN = 0x02,
    LEFT = 0x03,
    RIGHT = 0x04,
    UNKNOWN = 0x00
};

struct ActionDTO {
    ActionType type;

    /* Mover */
    Direction direction;
    /* Disparar */
    std::vector<uint16_t> desired_position;
    /* Update */
    std::vector<ObjectDTO> objects;

    uint16_t id;

    ActionDTO():
            type(ActionType::UNKNOWN),
            direction(Direction::UNKNOWN),
            desired_position(),
            objects(),
            id() {}

    explicit ActionDTO(const ActionType& action):
            type(action), direction(Direction::UNKNOWN), desired_position(), objects(), id() {}

    /* client -> server */
    /* Mover */
    ActionDTO(const ActionType& action, const Direction& direction):
            type(action), direction(direction), desired_position(), objects(), id() {}
    /* Mover con id (en server)*/
    ActionDTO(const ActionType& action, const Direction& direction, uint16_t id):
            type(action), direction(direction), desired_position(), objects(), id(id) {}

    /* Disparar */
    ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position):
            type(action), direction(), desired_position(desired_position), objects(), id() {}
    /* Disparar con id (en server)*/
    ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position, uint16_t id):
            type(action), direction(), desired_position(desired_position), objects(), id(id) {}

    /* server -> client */
    /* Update */
    ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects):
            type(action), direction(), desired_position(), objects(objects), id() {}
};

#endif  // ACTION_DTO_H
