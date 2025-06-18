#include "map_loader.h"

#include <filesystem>

#include "common/constants.h"

/* Getters */
std::vector<std::string> MapLoader::get_maps() const {
    std::vector<std::string> maps_vector;
    for (const auto& entry: std::filesystem::directory_iterator(MAPS_PATH)) {
        if (entry.is_regular_file() && entry.path().extension() == YAML_EXTENSION) {
            maps_vector.push_back(entry.path().stem().string());
        }
    }
    return maps_vector;
}
