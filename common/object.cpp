#include "object.h"

/* Constructores */
Object::Object(ObjectType object_type, uint16_t id, const std::vector<uint16_t>& position,
        uint16_t width, uint16_t height):
        object_type(object_type), id(id), position(position), width(width), height(height) {}

Object::Object(): object_type(ObjectType::UNKNOWN), id(0), position({0, 0}), width(0), height(0) {}

/* Getters */
ObjectType Object::get_type() const { return object_type; }
uint16_t Object::get_id() const { return id; }
uint16_t Object::get_width() const { return width; }
uint16_t Object::get_height() const { return height; }
std::vector<uint16_t> Object::get_position() const { return position; }


/* Funcionalidad */
void Object::move(const std::vector<uint16_t>& new_position) { position = new_position; }