#ifndef OBSTACLE_H
#define OBSTACLE_H

struct Obstacle {
    int x;
    int y;
    int height;
    int width;

    // si no definen alto y ancho, se considera un obstáculo de 1x1
    Obstacle(int x, int y) : x(x), y(y), height(1), width(1) {}
    
    // si definen alto y ancho, se considera un obstáculo de alto x ancho
    Obstacle(int x, int y, int height, int width) : x(x), y(y), height(height), width(width) {}

    Obstacle(const Obstacle& other) : x(other.x), y(other.y), height(other.height), width(other.width) {}
};

#endif  // OBSTACLE_H