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

const std::vector<ObstacleConfig> Config::get_obstacles() const { return obstacles; }

const std::vector<BombZoneConfig> Config::get_bomb_zones() const { return bomb_zones; }

const std::vector<TeamZoneConfig>& Config::get_team_zones() const { return team_zones; }

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

ObstacleType Config::box_to_obstacle_type(const std::string& type_string) {
    if (type_string == "CAJA1")
        return ObstacleType::OBSTACLE1;
    if (type_string == "CAJA2")
        return ObstacleType::OBSTACLE2;
    if (type_string == "CAJA3")
        return ObstacleType::OBSTACLE3;
    if (type_string == "CAJA4")
        return ObstacleType::OBSTACLE4;
    if (type_string == "CAJA5")
        return ObstacleType::OBSTACLE5;
    return ObstacleType::UNKNOWN;
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

    // Obstáculos
    if (config["obstacles"]) {
        for (const auto& node: config["obstacles"]) {
            ObstacleConfig obs;
            obs.width = node["width"].as<uint16_t>();
            obs.height = node["height"].as<uint16_t>();
            obs.type = box_to_obstacle_type(node["tipo"].as<std::string>());
            obs.x = node["position"]["x"].as<uint16_t>();
            obs.y = node["position"]["y"].as<uint16_t>();
            obstacles.push_back(obs);
        }
    }

    // Zonas de bomba
    if (config["bomb_zones"]) {
        for (const auto& node: config["bomb_zones"]) {
            BombZoneConfig bomb_zone;
            bomb_zone.x = node["position"]["x"].as<uint16_t>();
            bomb_zone.y = node["position"]["y"].as<uint16_t>();
            bomb_zone.width = node["width"].as<uint16_t>();
            bomb_zone.height = node["height"].as<uint16_t>();
            bomb_zones.push_back(bomb_zone);
        }
    }

    // Zonas de equipo
    if (config["team_zones"]) {
        for (const auto& node: config["team_zones"]) {
            TeamZoneConfig zone;
            std::string team_str = node["team"].as<std::string>();
            if (team_str == "TERRORIST") {
                zone.team = PlayerType::TERRORIST;
            } else if (team_str == "COUNTERTERRORIST") {
                zone.team = PlayerType::COUNTERTERRORIST;
            } else {
                zone.team = PlayerType::UNKNOWN;
            }
            zone.x = node["position"]["x"].as<uint16_t>();
            zone.y = node["position"]["y"].as<uint16_t>();
            zone.width = node["width"].as<uint16_t>();
            zone.height = node["height"].as<uint16_t>();
            team_zones.push_back(zone);
        }
    }
}
