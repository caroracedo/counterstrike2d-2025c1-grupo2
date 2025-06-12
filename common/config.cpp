#include "config.h"

/* Constructor */
Config::Config(const std::string& yaml_path) { load_from_yaml(yaml_path); }

/* Getters */
std::string Config::get_server_port() const { return server_port; }

uint8_t Config::get_player_health() const { return player_health; }

uint16_t Config::get_player_money() const { return player_money; }

uint8_t Config::get_rounds_total() const { return rounds_total; }

uint8_t Config::get_rounds_terrorist() const { return rounds_terrorists; }

uint8_t Config::get_rounds_counterterrorist() const { return rounds_counterterrorists; }

std::unordered_map<WeaponModel, WeaponConfig> Config::get_weapon_config() const {
    return weapon_catalog;
}

std::vector<WeaponModel> Config::get_weapons() const {
    std::vector<WeaponModel> weapons;
    std::transform(weapon_catalog.begin(), weapon_catalog.end(), std::back_inserter(weapons),
                   [](const auto& pair) { return pair.first; });
    return weapons;
}

WeaponModel Config::weapon_name_to_weapon_model(const std::string& weapon_name) {
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

void Config::load_from_yaml(const std::string& yaml_path) {
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
        if (config["rounds"]["terrorists"])
            rounds_terrorists = config["rounds"]["terrorists"].as<uint8_t>();
        if (config["rounds"]["counterterrorists"])
            rounds_counterterrorists = config["rounds"]["counterterrorists"].as<uint8_t>();
    }

    // Armas
    if (config["weapons"]) {
        for (const auto& it: config["weapons"]) {
            std::string weapon_name = it.first.as<std::string>();
            WeaponModel weapon_model = weapon_name_to_weapon_model(weapon_name);

            WeaponConfig weapon_config;
            weapon_config.price = it.second["price"].as<uint16_t>();
            weapon_config.range = it.second["range"].as<uint16_t>();
            weapon_config.min_damage = it.second["min_damage"].as<uint16_t>();
            weapon_config.max_damage = it.second["max_damage"].as<uint16_t>();
            weapon_config.precision = it.second["precision"].as<float>();
            weapon_catalog[weapon_model] = weapon_config;
        }
    }
}
