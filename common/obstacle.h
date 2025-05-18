#ifndef OBSTACLE_H
#define OBSTACLE_H

struct Obstacle {
    uint16_t x;
    uint16_t y;
    uint16_t height;
    uint16_t width;

    // si no definen alto y ancho, se considera un obstáculo de 1x1
    Obstacle(uint16_t x, uint16_t y): x(x), y(y), height(1), width(1) {}

    // si definen solo alto, se considera un obstáculo de alto x alto
    Obstacle(uint16_t x, uint16_t y, uint16_t height): x(x), y(y), height(height), width(height) {}

    // si definen alto y ancho, se considera un obstáculo de alto x ancho
    Obstacle(uint16_t x, uint16_t y, uint16_t height, uint16_t width): x(x), y(y), height(height), width(width) {}

    Obstacle(const Obstacle& other):
            x(other.x), y(other.y), height(other.height), width(other.width) {}
};

#endif  // OBSTACLE_H
