#ifndef MAP_H
#define MAP_H

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "types.h"
#include "weapon_DTO.h"

/* Obstáculos */
struct ObstacleConfig {
    uint16_t width;
    uint16_t height;
    ObstacleType type;
    uint16_t x;
    uint16_t y;
};

/* Weapons */
struct WeaponObjectConfig {
    WeaponModel type;
    uint16_t x;
    uint16_t y;
};

/* Zonas de bomba */
struct BombZoneConfig {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

/* Zonas de equipo */
struct TeamZoneConfig {
    PlayerType team;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

class Map {
private:
    /* Obstáculos */
    std::vector<ObstacleConfig> obstacles;

    /* Weapon Objects */
    std::vector<WeaponObjectConfig> weapon_objects;

    /* Zonas de bomba */
    std::vector<BombZoneConfig> bomb_zones;

    /* Zonas de equipo */
    std::vector<TeamZoneConfig> team_zones;

    ObstacleType box_to_obstacle_type(const std::string& type_string);

    WeaponModel weapon_type_to_weapon_model(const std::string& type_string);

    void load_from_yaml(const std::string& yaml_path);

public:
    /* Constructor */
    explicit Map(const std::string& yaml_path);

    /* Getters */
    const std::vector<ObstacleConfig>& get_obstacles() const;
    const std::vector<BombZoneConfig>& get_bomb_zones() const;
    const std::vector<TeamZoneConfig>& get_team_zones() const;
    const std::vector<WeaponObjectConfig>& get_weapon_objects() const;
};

#endif  // MAP_H
