#ifndef ACTION_DTO_H
#define ACTION_DTO_H

#include <cstdint>
#include <vector>

#include "weapon_DTO.h"

// cambio los valores de los action types para que se correspondan con la primera letra del comando
enum class ActionType : uint8_t { MOVE = 0x6D, SHOOT = 0x73, QUIT = 0x71, UNKNOWN = 0x75 };

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
    std::vector<uint16_t> position;
    WeaponDTO weapon;

    // para unknown
    ActionDTO(): type(ActionType::UNKNOWN), direction(), position(), weapon() {}

    explicit ActionDTO(const ActionType& action): type(action), direction(), position(), weapon() {}

    // para mover -- cliente -> server
    ActionDTO(const ActionType& action, const Direction& direction):
            type(action), direction(direction), position() {}

    ActionDTO(const Direction& direction, const WeaponDTO& weapon):
            type(ActionType::SHOOT), direction(direction), position(), weapon(weapon) {}

    // para que el cliente sepa donde está actualmente -- server -> cliente
    ActionDTO(const ActionType& action, const std::vector<uint16_t>& position):
            type(action), direction(), position(position) {}
};

#endif  // ACTION_DTO_H
