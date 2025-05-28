#ifndef OBJECT_H
#define OBJECT_H

#include <utility>
#include <vector>

#include "types.h"

class Object {
protected:
    ObjectType object_type;
    uint16_t id;
    std::vector<uint16_t> position;
    uint16_t width;
    uint16_t height;

public:
    /* Constructores */
    Object(ObjectType object_type, uint16_t id, const std::vector<uint16_t>& position,
           uint16_t width, uint16_t height):
            object_type(object_type), id(id), position(position), width(width), height(height) {}

    // Esto se usa?
    Object(): object_type(ObjectType::UNKNOWN), id(0), width(0), height(0), position({0, 0}) {}

    /* Getters */
    ObjectType get_type() const { return object_type; }
    uint16_t get_id() const { return id; }
    uint16_t get_width() const { return width; }
    uint16_t get_height() const { return height; }
    std::vector<uint16_t> get_position() const { return position; }


    /* Funcionalidad */
    void move(const std::vector<uint16_t>& new_position) { position = new_position; }

    // Esto no se...
    template <typename T>
    bool operator<(const T& other) const {
        if (id != other.id)
            return id < other.id;
        if (static_cast<uint8_t>(object_type) != static_cast<uint8_t>(other.object_type))
            return static_cast<uint8_t>(object_type) < static_cast<uint8_t>(other.object_type);
        if (position != other.position)
            return position < other.position;
        if (width != other.width)
            return width < other.width;
        return height < other.height;
    }

    // A este si le veo sentido
    bool operator==(const Object& other) const {
        return id == other.id && object_type == other.object_type;
    }
};

#endif  // OBJECT_H
