#include <iostream>
#include "game.h"

int main() {
    Game game;

    std::cout << "Posición inicial: ";
    for (auto v : game.get_position(1)) std::cout << v << " ";
    std::cout << std::endl; 

        // Intentar mover hacia la izquierda (puede fallar si está en el borde)
    if (game.move(Direction::LEFT, 1)) {
        std::cout << "\t\tMovido a la izquierda: ";
        for (auto v : game.get_position(1)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\t\tNo se pudo mover a la izquierda" << std::endl;
    }

    // Intentar mover hacia la derecha
    if (game.move(Direction::RIGHT, 1)) {
        std::cout << "\t\tMovido a la derecha: ";
        for (auto v : game.get_position(1)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\t\tNo se pudo mover a la derecha" << std::endl;
    }

    // Intentar mover hacia abajo
    if (game.move(Direction::DOWN, 1)) {
        std::cout << "\t\tMovido hacia abajo: ";
        for (auto v : game.get_position(1)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\t\tNo se pudo mover hacia abajo" << std::endl;
    }

    // Intentar mover hacia la izquierda (puede fallar si está en el borde)
    if (game.move(Direction::LEFT, 1)) {
        std::cout << "\t\tMovido a la izquierda: ";
        for (auto v : game.get_position(1)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\t\tNo se pudo mover a la izquierda" << std::endl;
    }

    // Intentar mover hacia arriba (puede fallar si está en el borde)
    if (game.move(Direction::UP, 1)) {
        std::cout << "\t\tMovido hacia arriba: ";
        for (auto v : game.get_position(1)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\t\tNo se pudo mover hacia arriba" << std::endl;
    }

    
    // Intentar mover hacia la derecha
    if (game.move(Direction::RIGHT, 1)) {
        std::cout << "\t\tMovido a la derecha: ";
        for (auto v : game.get_position(1)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\t\tNo se pudo mover a la derecha" << std::endl;
    }

        // Intentar mover hacia la derecha
    if (game.move(Direction::RIGHT, 1)) {
        std::cout << "\t\tMovido a la derecha: ";
        for (auto v : game.get_position(1)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\t\tNo se pudo mover a la derecha" << std::endl;
    }

            // Intentar mover hacia la derecha
    if (game.move(Direction::RIGHT, 1)) {
        std::cout << "\t\tMovido a la derecha: ";
        for (auto v : game.get_position(1)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\t\tNo se pudo mover a la derecha" << std::endl;
    }

    // Mostrar los objetos en la matriz
    std::vector<ObjectDTO> objects = game.get_objects();
    std::cout << "Objetos al finalizar" << std::endl;
    for (const auto& obj : objects) {
        std::cout << "\tObjeto ID: " << obj.id << ", Tipo: " << static_cast<int>(obj.type)
                  << ", Posición: (" << obj.position[0] << ", " << obj.position[1] << ")"
                  << std::endl;
    }

    return 0;
}

