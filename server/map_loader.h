#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <string>
#include <vector>

class MapLoader {
public:
    /* Getters */
    std::vector<std::string> get_maps() const;
};

#endif  // MAP_LOADER_H
