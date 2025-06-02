#ifndef BOMB_H
#define BOMB_H
#include "object.h"
#include "object_DTO.h"
#define BOMB_RADIUS 16

class Bomb: public Object {
    public:
    explicit Bomb(const std::vector<uint16_t>& position):
        Object(ObjectType::BOMB, 0, position, BOMB_RADIUS, BOMB_RADIUS) {}  
    
    ObjectDTO get_dto() const override {
        return ObjectDTO(object_type, position);
    }
};

#endif  // BOMB_H