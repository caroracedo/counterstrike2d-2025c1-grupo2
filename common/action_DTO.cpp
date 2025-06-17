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
ActionDTO::ActionDTO(const ActionType& action): ActionDTO() { this->type = action; }

/* Action con uint16_t */
ActionDTO::ActionDTO(ActionType action, uint16_t value): ActionDTO() {
    this->type = action;
    switch (type) {
        case ActionType::BOMB:
        case ActionType::CHANGE:
        case ActionType::PICKUP:
        case ActionType::QUIT:
            this->id = value;
            break;
        case ActionType::AMMOPRIMARY:
        case ActionType::AMMOSECONDARY:
            this->ammo = value;
            break;
        default:
            break;
    }
}

/* client -> server */
/* Crear Partida */
ActionDTO::ActionDTO(const ActionType& action, const std::string& match, const std::string& map,
                     const PlayerType& player_type):
        ActionDTO() {
    this->type = action;
    this->match = match;
    this->map = map;
    this->player_type = player_type;
}
/* Crear Partida con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const std::string& match, const std::string& map,
                     const PlayerType& player_type, uint16_t id):
        ActionDTO() {
    this->type = action;
    this->match = match;
    this->map = map;
    this->player_type = player_type;
    this->id = id;
}

/* Unirse a una Partida */
ActionDTO::ActionDTO(const ActionType& action, const std::string& match,
                     const PlayerType& player_type):
        ActionDTO() {
    this->type = action;
    this->match = match;
    this->player_type = player_type;
}
/* Unirse a una Partida con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const std::string& match,
                     const PlayerType& player_type, uint16_t id):
        ActionDTO() {
    this->type = action;
    this->match = match;
    this->player_type = player_type;
    this->id = id;
}

/* Mover */
ActionDTO::ActionDTO(const ActionType& action, const Direction& direction): ActionDTO() {
    this->type = action;
    this->direction = direction;
}
/* Mover con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const Direction& direction, uint16_t id):
        ActionDTO() {
    this->type = action;
    this->direction = direction;
    this->id = id;
}

/* Disparar */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position):
        ActionDTO() {
    this->type = action;
    this->desired_position = desired_position;
}
/* Disparar con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position,
                     uint16_t id):
        ActionDTO() {
    this->type = action;
    this->desired_position = desired_position;
    this->id = id;
}


/* Comprar Weapon */
ActionDTO::ActionDTO(const ActionType& action, WeaponModel weapon): ActionDTO() {
    this->type = action;
    this->weapon = weapon;
}
/* Comprar Weapon con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, WeaponModel weapon, uint16_t id): ActionDTO() {
    this->type = action;
    this->weapon = weapon;
    this->id = id;
}

/* Comprar Ammo */
ActionDTO::ActionDTO(const ActionType& action, uint16_t ammo, WeaponType weapon_type): ActionDTO() {
    this->type = action;
    this->ammo = ammo;
    this->weapon_type = weapon_type;
}
/* Comprar Ammo con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, uint16_t ammo, WeaponType weapon_type, uint16_t id):
        ActionDTO() {
    this->type = action;
    this->ammo = ammo;
    this->weapon_type = weapon_type;
    this->id = id;
}

/* Rotar */
ActionDTO::ActionDTO(const ActionType& action, float angle): ActionDTO() {
    this->type = action;
    this->angle = angle;
}
/* Rotar con id (en server)*/
ActionDTO::ActionDTO(const ActionType& action, float angle, uint16_t id): ActionDTO() {
    this->type = action;
    this->angle = angle;
    this->id = id;
}

/* server -> client */
/* Information */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<std::string>& matches,
                     const std::vector<std::string>& maps):
        ActionDTO() {
    this->type = action;
    this->matches = matches;
    this->maps = maps;
}

/* Configuration */
ActionDTO::ActionDTO(const ActionType& action, const TerrainType& terrain_type, uint16_t id):
        ActionDTO() {
    this->type = action;
    this->terrain_type = terrain_type;
    this->id = id;
}

/* Update */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects): ActionDTO() {
    this->type = action;
    this->objects = objects;
}

/* Shop */
ActionDTO::ActionDTO(const ActionType& action, const std::vector<WeaponModel>& weapons):
        ActionDTO() {
    this->type = action;
    this->weapons = weapons;
}

/* Stats */
ActionDTO::ActionDTO(const ActionType& action, const Stats& stats): ActionDTO() {
    this->type = action;
    this->stats = stats;
}
