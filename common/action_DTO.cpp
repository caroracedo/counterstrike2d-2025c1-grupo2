#include "action_DTO.h"

/* Comunes */
/* Unknown */
ActionDTO::ActionDTO():
        type(ActionType::UNKNOWN),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id() {}

/* Action */
ActionDTO::ActionDTO(const ActionType& action):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Action con uint16_t */
ActionDTO::ActionDTO(ActionType action, uint16_t value):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id() {
    switch (type) {
        case ActionType::PLAYERID:
        case ActionType::BOMB:
        case ActionType::CHANGE:
            id = value;
            break;
        case ActionType::AMMOPRIMARY:
        case ActionType::AMMOSECONDARY:
            ammo = value;
            break;
        default:
            break;
    }
}

/* client -> server */
/* Partida */
ActionDTO::ActionDTO(const ActionType& action, const std::string& match,
                     const PlayerType& player_type):
        type(action),
        match(match),
        player_type(player_type),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Partida con id (en server) */
ActionDTO::ActionDTO(const ActionType& action, const std::string& match,
                     const PlayerType& player_type, uint16_t id):
        type(action),
        match(match),
        player_type(player_type),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id(id) {}

/* Tipo de Jugador */
ActionDTO::ActionDTO(const ActionType& action, const PlayerType& player_type):
        type(action),
        match(),
        player_type(player_type),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Tipo de Jugador con id (en server) */
ActionDTO::ActionDTO(const ActionType& action, const PlayerType& player_type, uint16_t id):
        type(action),
        match(),
        player_type(player_type),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id(id) {}


/* Mover */
ActionDTO::ActionDTO(const ActionType& action, const Direction& direction):
        type(action),
        match(),
        player_type(),
        direction(direction),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Mover con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const Direction& direction, uint16_t id):
        type(action),
        match(),
        player_type(),
        direction(direction),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id(id) {}


/* Disparar */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(desired_position),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Disparar con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position,
                     uint16_t id):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(desired_position),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id(id) {}


/* Comprar Weapon */
ActionDTO::ActionDTO(const ActionType& action, WeaponModel weapon):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(weapon),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Comprar Weapon con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, WeaponModel weapon, uint16_t id):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(weapon),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(),
        id(id) {}

/* Comprar Ammo */
ActionDTO::ActionDTO(const ActionType& action, uint16_t ammo, WeaponType weapon_type):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(ammo),
        weapon_type(weapon_type),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Comprar Ammo con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, uint16_t ammo, WeaponType weapon_type, uint16_t id):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(ammo),
        weapon_type(weapon_type),
        objects(),
        weapons(),
        stats(),
        id(id) {}


/* server -> client */
/* Update */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(objects),
        weapons(),
        stats(),
        id() {}

/* Shop */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<WeaponModel>& weapons):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(weapons),
        stats(),
        id() {}

/* Stats */
ActionDTO::ActionDTO(const ActionType& action, const Stats& stats):
        type(action),
        match(),
        player_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        objects(),
        weapons(),
        stats(stats),
        id() {}
