#ifndef OBJECT_H
#define OBJECT_H

#include <cstdint>
#include <utility>
#include <vector>

#include "object_type.h"

class Object {
protected:
    ObjectType type;
    uint16_t id;
    uint16_t width;
    uint16_t height;
    std::vector<uint16_t> position;

public:
    Object(ObjectType type, uint16_t id, uint16_t width, uint16_t height,
           const std::vector<uint16_t>& position):
            type(type), id(id), width(width), height(height), position(position) {}

    ObjectType get_type() const { return type; }
    uint16_t get_id() const { return id; }
    uint16_t get_width() const { return width; }
    uint16_t get_height() const { return height; }
    std::vector<uint16_t> get_position() const { return position; }

    void move(const std::vector<uint16_t>& new_position) { position = new_position; }

    // Ensure ObjectDTO has a compatible 'type' member of ObjectType or cast appropriately
    template <typename T>
    bool operator<(const T& other) const {
        if (id != other.id)
            return id < other.id;
        if (static_cast<uint8_t>(type) != static_cast<uint8_t>(other.type))
            return static_cast<uint8_t>(type) < static_cast<uint8_t>(other.type);
        if (position != other.position)
            return position < other.position;
        if (width != other.width)
            return width < other.width;
        return height < other.height;
    }

    bool operator==(const Object& other) const { return id == other.id && type == other.type; }
};

#endif  // OBJECT_H
