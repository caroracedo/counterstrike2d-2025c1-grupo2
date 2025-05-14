#ifndef ACTION_DTO_H
#define ACTION_DTO_H

#include <cstdint>
#include <vector>

enum class ActionType : uint8_t { MOVE = 0x70, UNKNOWN };
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
            type(action), direction(), position(position) {}
};

#endif  // ACTION_DTO_H
