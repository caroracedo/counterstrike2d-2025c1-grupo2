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
    Direction direction;
    std::vector<ObjectDTO> objects;
    uint16_t id;

    // para unknown
    ActionDTO(): type(ActionType::UNKNOWN), direction(), objects(), id() {}

    explicit ActionDTO(const ActionType& action): type(action), direction(), objects(), id() {}

    // para mover -- cliente -> server
    ActionDTO(const ActionType& action, const Direction& direction):
            type(action), direction(direction), objects(), id() {}
    ActionDTO(const ActionType& action, const Direction& direction, uint16_t id):
            type(action), direction(direction), objects(), id(id) {}

    // para mandar el update
    ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects):
            type(action), direction(), objects(objects), id() {}
};

#endif  // ACTION_DTO_H
