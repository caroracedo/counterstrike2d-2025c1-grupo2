#ifndef CONFIG_H
#define CONFIG_H

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "types.h"

/* Armas */
struct WeaponConfig {
    uint16_t price;
    uint16_t range;
    uint16_t min_damage;
    uint16_t max_damage;
    float precision;
};

/* Obstáculos */
struct ObstacleConfig {
    uint16_t width;
    uint16_t height;
    ObstacleType type;
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

class Config {
private:
    /* Server */
    std::string server_port;

    /* Jugador */
    uint8_t player_health;
    uint16_t player_money;

    /* Rondas */
    uint8_t rounds_total;
    uint8_t rounds_switch;

    /* Armas */
    std::unordered_map<WeaponModel, WeaponConfig> weapon_catalog;

    /* Obstáculos */
    std::vector<ObstacleConfig> obstacles;

    /* Zonas de bomba */
    std::vector<BombZoneConfig> bomb_zones;

    /* Zonas de equipo */
    std::vector<TeamZoneConfig> team_zones;

    WeaponModel weapon_name_to_weapon_model(const std::string& weapon_name);

    ObstacleType box_to_obstacle_type(const std::string& type_string);

    void load_from_yaml(const std::string& yaml_path);

public:
    /* Constructor */
    explicit Config(const std::string& yaml_path);

    /* Getters */
    std::string get_server_port() const;
    uint8_t get_player_health() const;
    uint16_t get_player_money() const;
    uint8_t get_rounds_total() const;
    uint8_t get_rounds_switch() const;
    const std::vector<ObstacleConfig> get_obstacles() const;
    const std::vector<BombZoneConfig> get_bomb_zones() const;
    const std::vector<TeamZoneConfig>& get_team_zones() const;
    std::unordered_map<WeaponModel, WeaponConfig> get_weapon_config() const;
    std::vector<WeaponModel> get_weapons() const;
};

#endif  // CONFIG_H
