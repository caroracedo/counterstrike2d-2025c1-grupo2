#ifndef OBJECT_DTO_H
#define OBJECT_DTO_H

#include <cstdint>
#include <vector>

enum class ObjectType : uint8_t { PLAYER, OBSTACLE, WEAPON, UNKNOWN };

struct ObjectDTO {
    ObjectType type;
    std::vector<uint16_t> position;
    std::uint16_t width;
    std::uint16_t height;
    uint16_t id;

    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
              uint16_t height, uint16_t id):
            type(type), position(position), width(width), height(height), id(id) {}
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
              uint16_t height):
            type(type), position(position), width(width), height(height), id() {}

    // TODO: Revisar porque por ahora ignoro el width y height

    // Para el jugador por ahora... (por id)
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, int id):
            type(type), position(position), width(), height(), id(id) {}

    // Para otros objetos
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position):
            type(type), position(position), width(), height(), id() {}
};

#endif  // OBJECT_DTO_H
