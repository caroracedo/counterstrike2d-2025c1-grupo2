#ifndef OBJECT_DTO_H
#define OBJECT_DTO_H

#include <cstdint>
#include <vector>

#include "types.h"

struct ObjectDTO {
    ObjectType type;
    std::vector<uint16_t> position;
    std::uint16_t width;
    std::uint16_t height;
    uint16_t id;

    // TODO: Revisar porque por ahora ignoro el width y height
    // Ahora todos tienen id, sólo que a algunos no les sirve
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t id):
            type(type), position(position), width(), height(), id(id) {}
};

#endif  // OBJECT_DTO_H
