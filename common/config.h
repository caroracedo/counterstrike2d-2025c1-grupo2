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
    uint16_t damage;
};

/* Obstáculos */
struct ObstacleConfig {
    uint16_t width;
    uint16_t height;
    uint16_t x;
    uint16_t y;
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

    WeaponModel weapon_name_to_weapon_model(const std::string& weapon_name) {
        static const std::unordered_map<std::string, WeaponModel> weapon_name_model = {
                {"knife", WeaponModel::KNIFE}, {"glock", WeaponModel::GLOCK},
                {"ak47", WeaponModel::AK47},   {"m3", WeaponModel::M3},
                {"awp", WeaponModel::AWP},     {"bomb", WeaponModel::BOMB}};

        auto it = weapon_name_model.find(weapon_name);
        if (it != weapon_name_model.end()) {
            return it->second;
        }
        return WeaponModel::UNKNOWN;
    }

    void load_from_yaml(const std::string& yaml_path) {
        YAML::Node config = YAML::LoadFile(yaml_path);

        // Server
        if (config["server"] && config["server"]["port"])
            server_port = config["server"]["port"].as<std::string>();

        // Jugador
        if (config["player"]) {
            if (config["player"]["health"])
                player_health = config["player"]["health"].as<uint8_t>();
            if (config["player"]["money"])
                player_money = config["player"]["money"].as<uint16_t>();
        }

        // Rondas
        if (config["rounds"]) {
            if (config["rounds"]["total"])
                rounds_total = config["rounds"]["total"].as<uint8_t>();
            if (config["rounds"]["switch"])
                rounds_switch = config["rounds"]["switch"].as<uint8_t>();
        }

        // Armas
        if (config["weapons"]) {
            for (const auto& it: config["weapons"]) {
                std::string weapon_name = it.first.as<std::string>();
                WeaponModel weapon_model = weapon_name_to_weapon_model(weapon_name);

                WeaponConfig weapon_config;
                weapon_config.price = it.second["price"].as<uint16_t>();
                weapon_config.range = it.second["range"].as<uint16_t>();
                weapon_config.damage = it.second["damage"].as<uint16_t>();
                weapon_catalog[weapon_model] = weapon_config;
            }
        }

        // Obstáculos
        if (config["obstacles"]) {
            for (const auto& node: config["obstacles"]) {
                ObstacleConfig obs;
                obs.width = node["width"].as<uint16_t>();
                obs.height = node["height"].as<uint16_t>();
                obs.x = node["position"]["x"].as<uint16_t>();
                obs.y = node["position"]["y"].as<uint16_t>();
                obstacles.push_back(obs);
            }
        }
    }

public:
    /* Constructor */
    explicit Config(const std::string& yaml_path) { load_from_yaml(yaml_path); }

    /* Getters */
    std::string get_server_port() const { return server_port; }
    uint8_t get_player_health() const { return player_health; }
    uint16_t get_player_money() const { return player_money; }
    uint8_t get_rounds_total() const { return rounds_total; }
    uint8_t get_rounds_switch() const { return rounds_switch; }
    const std::vector<ObstacleConfig> get_obstacles() const { return obstacles; }
    std::unordered_map<WeaponModel, WeaponConfig> get_weapon_config() const {
        return weapon_catalog;
    }
    std::vector<WeaponModel> get_weapons() const {
        std::vector<WeaponModel> weapons;
        std::transform(weapon_catalog.begin(), weapon_catalog.end(), std::back_inserter(weapons),
                       [](const auto& pair) { return pair.first; });
        return weapons;
    }
};

#endif  // CONFIG_H
