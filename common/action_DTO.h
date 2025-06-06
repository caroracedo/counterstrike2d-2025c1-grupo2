#ifndef ACTION_DTO_H
#define ACTION_DTO_H

#include <cstdint>
#include <vector>

#include "object_DTO.h"
#include "types.h"

enum class ActionType : uint8_t {
    MOVE = 0x6D,
    SHOOT = 0x73,
    BOMB = 0x66,
    QUIT = 0x71,
    UPDATE = 0x75,
    PLAYERTYPE = 0x70,
    PLAYERID = 0x69,
    END = 0x59,
    SHOP = 0x57,
    WEAPON = 0x56,
    AMMO = 0x55,
    CHANGE = 0x40,
    UNKNOWN = 0x00
};

enum class Direction : uint8_t {
    UP = 0x01,
    DOWN = 0x02,
    LEFT = 0x03,
    RIGHT = 0x04,
    UNKNOWN = 0x00
};

enum class ActionParamType { ID, AMMO, WEAPON };

struct ActionDTO {
    ActionType type;

    /* Tipo de Jugador */
    PlayerType player_type;
    /* Mover */
    Direction direction;
    /* Disparar */
    std::vector<uint16_t> desired_position;
    /* Comprar Weapon */
    WeaponModel weapon;
    /* Comprar Ammo */
    uint16_t ammo;

    /* Update */
    std::vector<ObjectDTO> objects;
    /* Shop */
    std::vector<WeaponModel> weapons;

    /* Id */
    uint16_t id;


    /* Comunes */
    /* Unknown */
    ActionDTO():
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
    explicit ActionDTO(const ActionType& action):
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
    ActionDTO(ActionType action, uint16_t value):
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
    ActionDTO(const ActionType& action, const PlayerType& player_type):
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
    ActionDTO(const ActionType& action, const PlayerType& player_type, uint16_t id):
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
    ActionDTO(const ActionType& action, const Direction& direction):
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
    ActionDTO(const ActionType& action, const Direction& direction, uint16_t id):
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
    ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position):
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
    ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position, uint16_t id):
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
    ActionDTO(const ActionType& action, WeaponModel weapon):
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
    ActionDTO(const ActionType& action, WeaponModel weapon, uint16_t id):
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
    ActionDTO(const ActionType& action, uint16_t ammo, uint16_t id):
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
    ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects):
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
    ActionDTO(const ActionType& action, const std::vector<WeaponModel>& weapons):
            type(action),
            player_type(),
            direction(),
            desired_position(),
            weapon(),
            ammo(),
            objects(),
            weapons(weapons),
            id() {}
};

#endif  // ACTION_DTO_H
