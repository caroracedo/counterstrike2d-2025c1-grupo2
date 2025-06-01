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
    PLAYERTYPE = 0x70,
    PLAYERID = 0x69,
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

    /* Tipo de Jugador */
    PlayerType player_type;
    /* Mover */
    Direction direction;
    /* Disparar */
    std::vector<uint16_t> desired_position;
    /* Update */
    std::vector<ObjectDTO> objects;

    /* Id */
    uint16_t id;

    ActionDTO():
            type(ActionType::UNKNOWN),
            player_type(),
            direction(),
            desired_position(),
            objects(),
            id() {}

    explicit ActionDTO(const ActionType& action):
            type(action), player_type(), direction(), desired_position(), objects(), id() {}

    /* client -> server */
    /* Tipo de Jugador */
    ActionDTO(const ActionType& action, const PlayerType& player_type):
            type(action),
            player_type(player_type),
            direction(),
            desired_position(),
            objects(),
            id() {}
    /* Tipo de Jugador con id (en server) */
    ActionDTO(const ActionType& action, const PlayerType& player_type, uint16_t id):
            type(action),
            player_type(player_type),
            direction(),
            desired_position(),
            objects(),
            id(id) {}

    /* Mover */
    ActionDTO(const ActionType& action, const Direction& direction):
            type(action),
            player_type(),
            direction(direction),
            desired_position(),
            objects(),
            id() {}
    /* Mover con id (en server)*/
    ActionDTO(const ActionType& action, const Direction& direction, uint16_t id):
            type(action),
            player_type(),
            direction(direction),
            desired_position(),
            objects(),
            id(id) {}

    /* Disparar */
    ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position):
            type(action),
            player_type(),
            direction(),
            desired_position(desired_position),
            objects(),
            id() {}
    /* Disparar con id (en server)*/
    ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position, uint16_t id):
            type(action),
            player_type(),
            direction(),
            desired_position(desired_position),
            objects(),
            id(id) {}

    /* server -> client */
    /* Id */
    ActionDTO(const ActionType& action, uint16_t id):
            type(action), player_type(), direction(), desired_position(), objects(), id(id) {}

    /* Update */
    ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects):
            type(action), player_type(), direction(), desired_position(), objects(objects), id() {}
};

#endif  // ACTION_DTO_H
