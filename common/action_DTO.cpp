#include "action_DTO.h"

/* Comunes */
/* Unknown */
ActionDTO::ActionDTO():
        type(ActionType::UNKNOWN),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        objects(),
        weapons(),
        id() {}

/* Action */
explicit ActionDTO::ActionDTO(const ActionType& action):
        type(action),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        objects(),
        weapons(),
        id() {}
/* Action con uint16_t */
ActionDTO::ActionDTO(ActionType action, uint16_t value):
        type(action),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        objects(),
        weapons(),
        id() {
    switch (type) {
        case ActionType::PLAYERID:
        case ActionType::BOMB:
            id = value;
            break;
        case ActionType::AMMO:
            ammo = value;
            break;
        default:
            break;
    }
}

/* client -> server */
/* Tipo de Jugador */
ActionDTO::ActionDTO(const ActionType& action, const PlayerType& player_type):
        type(action),
        player_type(player_type),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        objects(),
        weapons(),
        id() {}
/* Tipo de Jugador con id (en server) */
ActionDTO::ActionDTO(const ActionType& action, const PlayerType& player_type, uint16_t id):
        type(action),
        player_type(player_type),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        objects(),
        weapons(),
        id(id) {}


/* Mover */
ActionDTO::ActionDTO(const ActionType& action, const Direction& direction):
        type(action),
        player_type(),
        direction(direction),
        desired_position(),
        weapon(),
        ammo(),
        objects(),
        weapons(),
        id() {}
/* Mover con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const Direction& direction, uint16_t id):
        type(action),
        player_type(),
        direction(direction),
        desired_position(),
        weapon(),
        ammo(),
        objects(),
        weapons(),
        id(id) {}


/* Disparar */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position):
        type(action),
        player_type(),
        direction(),
        desired_position(desired_position),
        weapon(),
        ammo(),
        objects(),
        weapons(),
        id() {}
/* Disparar con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position, uint16_t id):
        type(action),
        player_type(),
        direction(),
        desired_position(desired_position),
        weapon(),
        ammo(),
        objects(),
        weapons(),
        id(id) {}


/* Comprar Weapon */
ActionDTO::ActionDTO(const ActionType& action, WeaponModel weapon):
        type(action),
        player_type(),
        direction(),
        desired_position(),
        weapon(weapon),
        ammo(),
        objects(),
        weapons(),
        id() {}
/* Comprar Weapon con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, WeaponModel weapon, uint16_t id):
        type(action),
        player_type(),
        direction(),
        desired_position(),
        weapon(weapon),
        ammo(),
        objects(),
        weapons(),
        id(id) {}

/* Comprar Ammo con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, uint16_t ammo, uint16_t id):
        type(action),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(ammo),
        objects(),
        weapons(),
        id(id) {}


/* server -> client */
/* Update */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects):
        type(action),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        objects(objects),
        weapons(),
        id() {}

/* Shop */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<WeaponModel>& weapons):
        type(action),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        objects(),
        weapons(weapons),
        id() {}