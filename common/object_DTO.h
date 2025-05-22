#ifndef OBJECT_DTO_H
#define OBJECT_DTO_H

#include <cstdint>
#include <vector>

enum class ObjectType : uint8_t { PLAYER, BULLET, OBSTACLE, UNKNOWN };

struct ObjectDTO {
    ObjectType type;
    std::vector<uint16_t> position;
    std::uint16_t width;
    std::uint16_t height;
    uint16_t id;
};

#endif  // OBJECT_DTO_H