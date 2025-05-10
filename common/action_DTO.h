#ifndef ACTION_DTO_H
#define ACTION_DTO_H

enum class ActionType { MOVE, UNKNOWN };
enum class Direction { UP, DOWN, LEFT, RIGHT, UNKNOWN };

struct ActionDTO {
    ActionType type;
    Direction direction;

    // para unknown
    ActionDTO(): type(ActionType::UNKNOWN), direction() {}

    // para feats futuros (ej: soltar bomba)
    // ActionDTO(const ActionType& action) : type(action) {};

    // para mover
    ActionDTO(const ActionType& action, const Direction& direction):
            type(action), direction(direction) {}
};

#endif  // ACTION_DTO_H
