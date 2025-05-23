#ifndef OBJECT_DTO_H
#define OBJECT_DTO_H

#include <cstdint>
#include <vector>

enum class ObjectType : uint8_t { PLAYER, BULLET, OBSTACLE, ITEM };

struct ObjectDTO {
    ObjectType type;
    std::vector<int> position;
    int id;

    // Para el jugador por ahora... (por id)
    ObjectDTO(ObjectType type, const std::vector<int>& position, int id):
            type(type), position(position), id(id) {}

    // Para otros objetos
    ObjectDTO(ObjectType type, const std::vector<int>& position):
            type(type), position(position), id() {}
};

#endif  // OBJECT_DTO_H
