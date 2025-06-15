#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

enum class ObjectType : uint8_t { BOMB, BOMBZONE, BULLET, KNIFE, PLAYER, OBSTACLE, UNKNOWN };
enum class WeaponModel : uint8_t { KNIFE, GLOCK, AK47, M3, AWP, BOMB, UNKNOWN };
enum class WeaponType : uint8_t { KNIFE, PRIMARY, SECONDARY, BOMB, UNKNOWN };
enum class PlayerType : uint8_t { TERRORIST, COUNTERTERRORIST, UNKNOWN };
enum class TerrainType : uint8_t { PUEBLOAZTECA, ZONAENTRENAMIENTO, DESIERTO };
enum class ObstacleType : uint8_t {
    OBSTACLE1,
    OBSTACLE2,
    OBSTACLE3,
    OBSTACLE4,
    OBSTACLE5,
    UNKNOWN
};

#endif  // TYPES_H
