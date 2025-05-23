#ifndef ACTION_DTO_H
#define ACTION_DTO_H

#include <cstdint>
#include <vector>

#include "object_DTO.h"

enum class ActionType : uint8_t { MOVE = 0x70, QUIT = 0x71, UPDATE = 0x72, UNKNOWN = 0xFF };
enum class Direction : uint8_t {
    UP = 0x01,
    DOWN = 0x02,
    LEFT = 0x03,
    RIGHT = 0x04,
    UNKNOWN = 0x00
};

struct ActionDTO {
    ActionType type;
    int id;
    Direction direction;
    std::vector<ObjectDTO> objects;

    // para unknown
    ActionDTO(): type(ActionType::UNKNOWN), id(), direction(), objects() {}

    explicit ActionDTO(const ActionType& action): type(action), id(), direction(), objects() {}

    // para feats futuros (ej: soltar bomba)
    // ActionDTO(const ActionType& action) : type(action) {};

    // cliente -> server
    ActionDTO(const ActionType& action, const Direction& direction):
            type(action), id(), direction(direction), objects() {}
    ActionDTO(const ActionType& action, int id, const Direction& direction):
            type(action), id(id), direction(direction), objects() {}

    // server -> cliente
    ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects):
            type(action), id(), direction(), objects(objects) {}
};

#endif  // ACTION_DTO_H
