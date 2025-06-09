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
    ActionDTO();

    /* Action */
    explicit ActionDTO(const ActionType& action);

    /* Action con uint16_t */
    ActionDTO(ActionType action, uint16_t value);

    /* client -> server */
    /* Tipo de Jugador */
    ActionDTO(const ActionType& action, const PlayerType& player_type);

    /* Tipo de Jugador con id (en server) */
    ActionDTO(const ActionType& action, const PlayerType& player_type, uint16_t id);

    /* Mover */
    ActionDTO(const ActionType& action, const Direction& direction);

    /* Mover con id (en server)*/
    ActionDTO(const ActionType& action, const Direction& direction, uint16_t id);

    /* Disparar */
    ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position);
    /* Disparar con id (en server)*/
    ActionDTO(const ActionType& action, const std::vector<uint16_t>& desired_position, uint16_t id);

    /* Comprar Weapon */
    ActionDTO(const ActionType& action, WeaponModel weapon);

    /* Comprar Weapon con id (en server)*/
    ActionDTO(const ActionType& action, WeaponModel weapon, uint16_t id);

    /* Comprar Ammo con id (en server)*/
    ActionDTO(const ActionType& action, uint16_t ammo, uint16_t id);

    /* server -> client */
    /* Update */
    ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects);

    /* Shop */
    ActionDTO(const ActionType& action, const std::vector<WeaponModel>& weapons);
};

#endif  // ACTION_DTO_H
