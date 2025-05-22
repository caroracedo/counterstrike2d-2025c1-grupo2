#ifndef OBJECT_DTO_H
#define OBJECT_DTO_H

#include <cstdint>
#include <vector>

enum class ObjectType : uint8_t { PLAYER, BULLET, WALL, ITEM };

struct ObjectDTO {
    ObjectType type;
    std::vector<uint16_t> position;
};

#endif  // OBJECT_DTO_H
