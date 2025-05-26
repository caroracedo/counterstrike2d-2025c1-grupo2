#ifndef ACTION_DTO2_H
#define ACTION_DTO2_H

#include <cstdint>
#include <vector>

#include "object_DTO.h"

enum class ActionType2 : uint8_t {
    MOVE = 0x6D,
    QUIT = 0x71,
    SHOOT = 0x73,
    UPDATE = 0x75,
    UNKNOWN = 0x00
};
enum class Direction2 : uint8_t {
    UP = 0x01,
    DOWN = 0x02,
    LEFT = 0x03,
    RIGHT = 0x04,
    UNKNOWN = 0x00
};

struct ActionDTO2 {
    ActionType2 type;
    Direction2 direction;
    std::vector<uint16_t> position;
    std::vector<ObjectDTO> objects;

    // para unknown
    ActionDTO2(): type(ActionType2::UNKNOWN), direction(), position() {}

    ActionDTO2(const ActionType2& action): type(action), direction(), position() {}

    // para mover -- cliente -> server
    ActionDTO2(const ActionType2& action, const Direction2& direction):
            type(action), direction(direction), position() {}

    // para que el cliente sepa donde está actualmente -- server -> cliente
    ActionDTO2(const ActionType2& action, const std::vector<uint16_t>& position):
            type(action), direction(), position(position) {}

    // para mandar el update
    ActionDTO2(const ActionType2& action, const std::vector<ObjectDTO>& objects):
            type(action), direction(), position(), objects(objects) {}
};

#endif  // ACTION_DTO2_H