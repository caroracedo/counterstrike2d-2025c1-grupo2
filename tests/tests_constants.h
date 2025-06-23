#ifndef TESTS_TESTS_CONSTANTS_H
#define TESTS_TESTS_CONSTANTS_H

#include "common/constants.h"
#include "common/object_DTO.h"
#include "common/types.h"

#define HOSTNAME "localhost"
#define SERVNAME "1500"
#define MATCH_1 "a"
#define MATCH_2 "b"
#define MATCH_3 "c"
#define MAP_1 "a"
#define MAP_2 "b"
#define MAP_1_TERRAIN_TYPE TerrainType::PUEBLOAZTECA
#define ID_1 1
#define TEST_TIME 100
#define WEAPON_1 WeaponModel::AK47
#define WEAPON_2 WeaponModel::M3
#define AMMO 10  // TODO: Esto me hizo acordar que se hardcodea el AMMO en InputHandler
#define POSITION \
    { 10, 10 }
#define PLAYER_OBJECT                                                                              \
    ObjectDTO(ObjectType::PLAYER, POSITION, ID_1, PlayerType::TERRORIST, WEAPON_1, 500, 100, AMMO, \
              0)
#define OBSTACLE_OBJECT                                                        \
    ObjectDTO(ObjectType::OBSTACLE, POSITION, OBSTACLE_HEIGHT, OBSTACLE_WIDTH, \
              ObstacleType::OBSTACLE1)
#define BOMB_OBJECT ObjectDTO(ObjectType::BOMB, POSITION, 10)
#define BOMB_ZONE_OBJECT ObjectDTO(ObjectType::BOMBZONE, POSITION, 32, 32)
#define BULLET_OBJECT ObjectDTO(ObjectType::BULLET, POSITION)
#define WEAPON_OBJECT ObjectDTO(ObjectType::WEAPON, POSITION, WEAPON_2)

#endif  // TESTS_TESTS_CONSTANTS_H
