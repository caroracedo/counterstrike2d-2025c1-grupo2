#include <iostream>

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

// void shoot(Game& game, Direction direction, uint16_t id, uint16_t range) {
//     auto result = game.shoot(direction, id, range);
//     if (result.first && range > 5) {
//         reshoot(game, direction, id, range - 5, result.second);
//     }
// }

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

    // std::cout <<
    // "\n*******************************************************************************"
    //              "*************************************"
    //           << std::endl;
    // std::cout << "***************************************************PRUEBA "
    //              "DISPARO***************************************************"
    //           << std::endl;
    // std::cout <<
    // "*********************************************************************************"
    //              "***********************************"
    //           << std::endl;

    // shoot(game, Direction::LEFT, 2, 10);
    // shoot(game, Direction::LEFT, 2, 10);
    // shoot(game, Direction::LEFT, 2, 10);
    // shoot(game, Direction::LEFT, 2, 10);
    // shoot(game, Direction::LEFT, 2, 10);

    std::cout << "\n*******************************************************************************"
                 "*************************************"
              << std::endl;

    show(game);


    return 0;
}
