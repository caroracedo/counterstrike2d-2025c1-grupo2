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
           uint16_t width, uint16_t height);

    Object();

    /* Virtual puro */
    virtual ObjectDTO get_dto() const = 0;  // Método virtual puro para obtener el DTO del objeto

    /* Getters */
    ObjectType get_type() const;
    uint16_t get_id() const;
    uint16_t get_width() const;
    uint16_t get_height() const;
    std::vector<uint16_t> get_position() const;


    /* Funcionalidad */
    void move(const std::vector<uint16_t>& new_position);

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

    bool operator==(const Object& other) const {
        return id == other.id && object_type == other.object_type;
    }

    virtual ~Object() = default;  // Destructor virtual para permitir herencia polimórfica
};

#endif  // OBJECT_H
