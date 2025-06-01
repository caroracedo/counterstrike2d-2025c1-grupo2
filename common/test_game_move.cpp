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

void shoot(Game& game, Direction direction, uint16_t id) {
    std::vector<uint16_t> position = game.get_position(id);
    switch (direction) {
        case Direction::UP:
            position[1] -= 40;
            break;
        case Direction::DOWN:
            position[1] += 40;
            break;
        case Direction::LEFT:
            position[0] -= 40;
            break;
        case Direction::RIGHT:
            position[0] += 40;
            break;
        default:
            break;
    }

    game.shoot(position, id);
}

void show(Game& game) { game.show_objects(); }

int main() {
    Game game;

    show(game);

    // std::cout <<
    // "*********************************************************************************"
    //              "***********************************"
    //           << std::endl;
    // std::cout << "***********************************************PRUEBA DE "
    //              "MOVIMIENTOS************************************************"
    //           << std::endl;
    // std::cout <<
    // "*********************************************************************************"
    //              "***********************************"
    //           << std::endl;

    // // Pruebo todos los movimientos del jugador -> no hay colisión
    // move_player(game, Direction::RIGHT, 1);
    // move_player(game, Direction::DOWN, 1);
    // move_player(game, Direction::LEFT, 1);
    // move_player(game, Direction::UP, 1);

    // std::cout <<
    // "\n*******************************************************************************"
    //              "*************************************"
    //           << std::endl;
    // std::cout << "******************************************PRUEBA DE COLISIÓN CON "
    //              "OBSTÁCULO******************************************"
    //           << std::endl;
    // std::cout <<
    // "*********************************************************************************"
    //              "***********************************"
    //           << std::endl;

    // // Colisión con un obstáculo
    // move_player(game, Direction::RIGHT, 1);
    // move_player(game, Direction::RIGHT, 1);

    // std::cout <<
    // "\n*******************************************************************************"
    //              "*************************************"
    //           << std::endl;
    // std::cout << "*********************************************PRUEBA DE COLISIÓN CON "
    //              "BALA********************************************"
    //           << std::endl;
    // std::cout <<
    // "*********************************************************************************"
    //              "***********************************"
    //           << std::endl;

    // move_player(game, Direction::DOWN, 1);
    // move_player(game, Direction::DOWN, 1);

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

    // for (int i = 0; i < 5; ++i) {
    //     shoot(game, Direction::LEFT, 2);
    //     for (int j = 0; j < 5; ++j) {
    //         game.update();
    //     }
    // }

    std::cout << "\n*******************************************************************************"
                 "*************************************"
              << std::endl;
    std::cout << "******************************************************PRUEBA DISPARO M3********"
                 "****************************************"
              << std::endl;
    std::cout << "*********************************************************************************"
                 "***********************************"
              << std::endl;

    // TODO: ver por qué con estas posiciones los angulos entre los vectores es distinto {20,100},
    // {200,30}, {60,0}, {15,0}, {90, 50}
    std::vector<std::vector<uint16_t>> positions = {{0, 0},     {0, 30},   {20, 100},
                                                    {100, 100}, {200, 30}, {60, 0},
                                                    {15, 0},    {20, 20},  {90, 50}};


    /*
    Valores esperados de los disparos:
            - Disparando a la posición: (0, 0)
                - Bala 1 (14, 14), posición objetivo: (4, 4)
                - Bala 2 (22, 10), posición objetivo: (20, 4)
                - Bala 3 (10, 22), posición objetivo: (4, 20)
            - Disparando a la posición: (0, 30)
                - Bala 4 (9, 30), posición objetivo: (4, 30)
                - Bala 5 (12, 20), posición objetivo: (4, 15)
                - Bala 6 (12, 40), posición objetivo: (4, 45)
       !    - Disparando a la posición: (20, 100)
                - Bala 7 (27, 51), posición objetivo: (21, 95)
                - Bala 8 (20, 48), posición objetivo: (4, 78)
                - Bala 9 (38, 49), posición objetivo: (55, 91)
            - Disparando a la posición: (100, 100)
                - Bala 10 (45, 45), posición objetivo: (77, 77)
                - Bala 11 (35, 50), posición objetivo: (47, 94)
                - Bala 12 (50, 35), posición objetivo: (94, 47)
       !    - Disparando a la posición: (200, 30)
                - Bala 13 (51, 30), posición objetivo: (96, 30)
                - Bala 14 (48, 41), posición objetivo: (87, 63)
                - Bala 15 (51, 26), posición objetivo: (95, 17)
       !    - Disparando a la posición: (60, 0)
                - Bala 16 (45, 15), posición objetivo: (56, 4)
                - Bala 17 (51, 24), posición objetivo: (94, 12)
                - Bala 18 (37, 10), posición objetivo: (39, 4)
       !    - Disparando a la posición: (15, 0)
                - Bala 19 (21, 11), posición objetivo: (17, 4)
                - Bala 20 (32, 9), posición objetivo: (32, 4)
                - Bala 21 (12, 17), posición objetivo: (4, 12)
            - Disparando a la posición: (20, 20)
                - Bala 22 (14, 14), posición objetivo: (4, 4)
                - Bala 23 (25, 10), posición objetivo: (23, 4)
                - Bala 24 (10, 25), posición objetivo: (4, 23)
       !    - Disparando a la posición: (90, 50)
                - Bala 25 (51, 37), posición objetivo: (93, 51)
                - Bala 26 (44, 46), posición objetivo: (73, 80)
                - Bala 27 (52, 25), posición objetivo: (95, 16)


    */
    for (const auto& pos: positions) {
        std::cout << "\t\tDisparando a la posición: (" << pos[0] << ", " << pos[1] << ")"
                  << std::endl;
        game.shoot(pos, 1);
        for (int j = 0; j < 20; ++j) {
            game.update();
        }
        std::cout << "*******************************" << std::endl;
    }


    std::cout << "\n*******************************************************************************"
                 "*************************************"
              << std::endl;

    show(game);


    return 0;
}
