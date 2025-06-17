#include "action_DTO.h"

/* Comunes */
/* Unknown */
ActionDTO::ActionDTO():
        type(ActionType::UNKNOWN),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {}

/* Action */
ActionDTO::ActionDTO(const ActionType& action):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Action con uint16_t */
ActionDTO::ActionDTO(ActionType action, uint16_t value):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {
    switch (type) {
        case ActionType::BOMB:
        case ActionType::CHANGE:
        case ActionType::PICKUP:
        case ActionType::QUIT:
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
/* Configuration */
ActionDTO::ActionDTO(const ActionType& action, const std::string& match, const std::string& map,
                     const PlayerType& player_type):
        type(action),
        match(match),
        map(map),
        player_type(player_type),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {}
ActionDTO::ActionDTO(const ActionType& action, const std::string& match,
                     const PlayerType& player_type):
        type(action),
        match(match),
        map(),
        player_type(player_type),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {}

/* Mover */
ActionDTO::ActionDTO(const ActionType& action, const Direction& direction):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(direction),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Mover con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const Direction& direction, uint16_t id):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(direction),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id(id) {}


/* Disparar */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(desired_position),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Disparar con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position,
                     uint16_t id):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(desired_position),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id(id) {}


/* Comprar Weapon */
ActionDTO::ActionDTO(const ActionType& action, WeaponModel weapon):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(weapon),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Comprar Weapon con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, WeaponModel weapon, uint16_t id):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(weapon),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id(id) {}

/* Comprar Ammo */
ActionDTO::ActionDTO(const ActionType& action, uint16_t ammo, WeaponType weapon_type):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(ammo),
        weapon_type(weapon_type),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Comprar Ammo con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, uint16_t ammo, WeaponType weapon_type, uint16_t id):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(ammo),
        weapon_type(weapon_type),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id(id) {}

/* Rotar */
ActionDTO::ActionDTO(const ActionType& action, float angle):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(angle),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id() {}
/* Rotar con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, float angle, uint16_t id):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(angle),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id(id) {}

/* server -> client */
/* Configuration */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<std::string>& matches,
                     const std::vector<std::string>& maps):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(matches),
        maps(maps),
        objects(),
        weapons(),
        stats(),
        id() {}
ActionDTO::ActionDTO(const ActionType& action, const TerrainType& terrain_type, uint16_t id):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(terrain_type),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id(id) {}
ActionDTO::ActionDTO(const ActionType& action, const std::string& match, const std::string& map,
                     const PlayerType& player_type, uint16_t id):
        type(action),
        match(match),
        map(map),
        player_type(player_type),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id(id) {}
ActionDTO::ActionDTO(const ActionType& action, const std::string& match,
                     const PlayerType& player_type, uint16_t id):
        type(action),
        match(match),
        map(),
        player_type(player_type),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(),
        id(id) {}

/* Update */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(objects),
        weapons(),
        stats(),
        id() {}

/* Shop */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<WeaponModel>& weapons):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(weapons),
        stats(),
        id() {}

/* Stats */
ActionDTO::ActionDTO(const ActionType& action, const Stats& stats):
        type(action),
        match(),
        map(),
        player_type(),
        terrain_type(),
        direction(),
        desired_position(),
        weapon(),
        ammo(),
        weapon_type(),
        angle(),
        matches(),
        maps(),
        objects(),
        weapons(),
        stats(stats),
        id() {}
