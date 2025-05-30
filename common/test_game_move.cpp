#include <cstdint>
#include <iostream>
#include <vector>

#include "game.h"

std::string direction_to_string(Direction direction) {
    switch (direction) {
        case Direction::UP:
            return "arriba";
        case Direction::DOWN:
            return "abajo";
        case Direction::LEFT:
            return "la izquierda";
        case Direction::RIGHT:
            return "la derecha";
        default:
            return "UNKNOWN";
    }
}

void move_player(Game& game, Direction direction, uint16_t id) {
    if (game.move(direction, id)) {
        std::cout << "\tJugador " << static_cast<int>(id) << " movido hacia "
                  << direction_to_string(direction) << ": ";
        for (auto v: game.get_position(id)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\tNo se pudo mover al jugador " << static_cast<int>(id) << " hacia "
                  << direction_to_string(direction) << std::endl;
    }
}

void shoot(Game& game, Direction direction, uint16_t id) {
    std::vector<uint16_t> position = game.get_position(id);
    switch (direction) {
        case Direction::UP:
            position[1] -= 40;  // Dispara hacia arriba
            break;
        case Direction::DOWN:
            position[1] += 40;  // Dispara hacia abajo
            break;
        case Direction::LEFT:
            position[0] -= 40;  // Dispara hacia la izquierda
            break;
        case Direction::RIGHT:
            position[0] += 40;  // Dispara hacia la derecha
            break;
        default:
            break;
    }
    game.shoot(position, id);
    std::cout << "\tJugador " << static_cast<int>(id) << " disparó hacia "
              << direction_to_string(direction) << std::endl;
}

void show(Game& game) { game.show_objects(); }

int main() {
    Game game;

    show(game);

    std::cout << "*********************************************************************************"
                 "***********************************"
              << std::endl;
    std::cout << "***********************************************PRUEBA DE "
                 "MOVIMIENTOS************************************************"
              << std::endl;
    std::cout << "*********************************************************************************"
                 "***********************************"
              << std::endl;

    // Pruebo todos los movimientos del jugador -> no hay colisión
    move_player(game, Direction::RIGHT, 1);
    move_player(game, Direction::DOWN, 1);
    move_player(game, Direction::LEFT, 1);
    move_player(game, Direction::UP, 1);

    std::cout << "\n*******************************************************************************"
                 "*************************************"
              << std::endl;
    std::cout << "******************************************PRUEBA DE COLISIÓN CON "
                 "OBSTÁCULO******************************************"
              << std::endl;
    std::cout << "*********************************************************************************"
                 "***********************************"
              << std::endl;

    // Colisión con un obstáculo
    move_player(game, Direction::RIGHT, 1);
    move_player(game, Direction::RIGHT, 1);

    std::cout << "\n*******************************************************************************"
                 "*************************************"
              << std::endl;
    std::cout << "*********************************************PRUEBA DE COLISIÓN CON "
                 "BALA********************************************"
              << std::endl;
    std::cout << "*********************************************************************************"
                 "***********************************"
              << std::endl;

    move_player(game, Direction::DOWN, 1);
    move_player(game, Direction::DOWN, 1);
    move_player(game, Direction::LEFT, 1);
    move_player(game, Direction::LEFT, 1);

    std::cout << "\n*******************************************************************************"
                 "*************************************"
              << std::endl;
    std::cout << "***************************************************PRUEBA "
                 "DISPARO***************************************************"
              << std::endl;
    std::cout << "*********************************************************************************"
                 "***********************************"
              << std::endl;

    for (int i = 0; i < 5; ++i) {
        shoot(game, Direction::LEFT, 2);
        for (int j = 0; j < 4; ++j) {
            game.update();
        }
    }


    shoot(game, Direction::UP, 2);

    std::cout << "\n*******************************************************************************"
                 "*************************************"
              << std::endl;

    show(game);


    return 0;
}
