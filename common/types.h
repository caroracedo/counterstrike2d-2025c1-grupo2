#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

enum class ObjectType : uint8_t { BOMB, BULLET, KNIFE, PLAYER, OBSTACLE, UNKNOWN };
enum class WeaponModel : uint8_t { KNIFE, GLOCK, AK47, M3, AWP, BOMB, UNKNOWN };
enum class WeaponType : uint8_t { KNIFE, PRIMARY, SECONDARY, BOMB, UNKNOWN };
enum class PlayerType : uint8_t { TERRORIST, COUNTERTERRORIST, UNKNOWN };

#endif  // TYPES_H
