#include "map.h"

#include <iostream>

/* Constructor */
Map::Map(const std::string& yaml_path) { load_from_yaml(yaml_path); }

/* Getters */
const std::vector<ObstacleConfig> Map::get_obstacles() const { return obstacles; }

const std::vector<BombZoneConfig> Map::get_bomb_zones() const { return bomb_zones; }

const std::vector<TeamZoneConfig>& Map::get_team_zones() const { return team_zones; }

ObstacleType Map::box_to_obstacle_type(const std::string& type_string) {
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

void Map::load_from_yaml(const std::string& yaml_path) {
    YAML::Node config = YAML::LoadFile(yaml_path);

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

    // Obtener terreno
    if (config["terreno"]) {
        std::string tipoTerreno = config["terreno"].as<std::string>();
        terrain = (tipoTerreno == "desierto"            ? TerrainType::DESIERTO :
                   tipoTerreno == "zonadeentrenamiento" ? TerrainType::ZONAENTRENAMIENTO :
                                                          TerrainType::PUEBLOAZTECA);
    }
}
