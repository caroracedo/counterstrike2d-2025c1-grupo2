#ifndef ACTION_DTO_H
#define ACTION_DTO_H

#include <cstdint>
#include <string>
#include <vector>

#include "object_DTO.h"
#include "stats.h"
#include "types.h"

enum class ActionType : uint8_t {
    MOVE = 0x6D,
    SHOOT = 0x73,
    BOMB = 0x66,
    QUIT = 0x71,
    UPDATE = 0x75,
    INFORMATION = 0x70,
    CONFIGURATION = 0x69,
    PICKUP = 0x70,
    END = 0x59,
    SHOP = 0x57,
    WEAPON = 0x56,
    AMMOPRIMARY = 0x55,
    AMMOSECONDARY = 0x54,
    CREATE = 0x42,
    JOIN = 0x43,
    CHANGE = 0x40,
    STATS = 0x9D,
    START = 0x3D,
    ROTATE = 0x2D,
    INIT = 0x10,
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

    /* Init */
    std::string hostname;
    std::string servname;

    /* Partida */
    std::string match;
    std::string map;
    uint16_t number_terrorist;
    uint16_t number_counterterrorist;
    PlayerType player_type;
    PlayerSkin player_skin;
    TerrainType terrain_type;
    /* Mover */
    Direction direction;
    /* Disparar */
    std::vector<uint16_t> desired_position;
    /* Comprar Weapon */
    WeaponModel weapon;
    /* Comprar Ammo */
    uint16_t ammo;
    WeaponType weapon_type;
    /* Rotar */
    float angle;

    /* Configuration */
    std::vector<std::string> matches;
    std::vector<std::string> maps;
    /* Update */
    std::vector<ObjectDTO> objects;
    /* Shop */
    std::vector<WeaponModel> weapons;
    /* Stats */
    Stats stats;

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
    /* Init */
    ActionDTO(const ActionType& action, const std::string& hostname, const std::string& servname);
    /* Crear Partida */
    ActionDTO(const ActionType& action, const std::string& match, const std::string& map,
              uint16_t number_terrorist, uint16_t number_counterterrorist,
              const PlayerType& player_type, const PlayerSkin& player_skin);

    /* Unirse a una Partida */
    ActionDTO(const ActionType& action, const std::string& match, const PlayerType& player_type,
              const PlayerSkin& player_skin);

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
    ActionDTO(const ActionType& action, uint16_t ammo, WeaponType weapon_type, uint16_t id);

    /* Rotar */
    ActionDTO(const ActionType& action, float angle);
    /* Rotar con id (en server)*/
    ActionDTO(const ActionType& action, float angle, uint16_t id);

    /* server -> client */
    /* Information */
    ActionDTO(const ActionType& action, const std::vector<std::string>& matches,
              const std::vector<std::string>& maps);
    /* Configuration */
    ActionDTO(const ActionType& action, const TerrainType& terrain_type, uint16_t id);

    /* Update */
    ActionDTO(const ActionType& action, const std::vector<ObjectDTO>& objects);

    /* Shop */
    ActionDTO(const ActionType& action, const std::vector<WeaponModel>& weapons);

    /* Stats */
    ActionDTO(const ActionType& action, const Stats& stats);
};

#endif  // ACTION_DTO_H
