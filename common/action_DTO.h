#ifndef ACTION_DTO_H
#define ACTION_DTO_H

#include <vector>

enum class ActionType { MOVE, UNKNOWN };
enum class Direction { UP, DOWN, LEFT, RIGHT, UNKNOWN };

struct ActionDTO {
    ActionType type;
    Direction direction;
    std::vector<uint8_t> position;

    // para unknown
    ActionDTO(): type(ActionType::UNKNOWN), direction(), position() {}

    // para feats futuros (ej: soltar bomba)
    // ActionDTO(const ActionType& action) : type(action) {};

    // para mover -- cliente -> server
    ActionDTO(const ActionType& action, const Direction& direction):
            type(action), direction(direction), position() {}

    // para que el cliente sepa donde está actualmente -- server -> cliente
    ActionDTO(const ActionType& action, const std::vector<uint8_t>& position):
            type(action), position(position), direction() {}
};

#endif  // ACTION_DTO_H
