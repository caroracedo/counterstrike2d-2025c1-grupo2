#ifndef OBJECT_DTO_H
#define OBJECT_DTO_H

#include <cstdint>
#include <vector>

#include "object_type.h"

struct ObjectDTO {
    ObjectType type;
    std::vector<uint16_t> position;
    uint16_t id;
    uint16_t width;
    uint16_t height;

    // Ahora todos tienen id, sólo que a algunos no les sirve
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t id):
            type(type), position(position), id(id), width(), height() {}
    
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t id, uint16_t width, uint16_t height):
            type(type), position(position), id(id), width(width), height(height) {}
};

#endif  // OBJECT_DTO_H
