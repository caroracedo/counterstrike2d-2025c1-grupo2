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
        std::cout << "\t\tJugador " << static_cast<int>(id) << " movido hacia "
                  << direction_to_string(direction) << ": ";
        for (auto v: game.get_position(id)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\t\tNo se pudo mover al jugador " << static_cast<int>(id) << " hacia "
                  << direction_to_string(direction) << std::endl;
    }
}

// void shoot(Game& game, Direction direction, uint16_t id, uint16_t range) {
//     auto result = game.shoot(direction, id, range);
//     if (result.first) {
//         std::cout << "\t\tDisparado hacia " << direction_to_string(direction) << std::endl;
//         reshoot(game, direction, id, range, result.second);

//     } else {
//         std::cout << "\t\tNo se pudo disparar hacia " << direction_to_string(direction) <<
//         std::endl;
//     }
// }

// void reshoot(Game& game, Direction direction, uint16_t id, uint16_t range, uint16_t b_id) {
//     auto result = game.shoot(direction, id, range, b_id);
//     if (result.first) {
//         std::cout << "\t\tDisparado hacia " << direction_to_string(direction) << std::endl;
//     } else {
//         std::cout << "\t\tNo se pudo disparar hacia " << direction_to_string(direction) <<
//         std::endl;
//     }
// }

int main() {
    Game game;

    std::cout << "Posición inicial: ";
    for (auto v: game.get_position(1)) std::cout << v << " ";
    std::cout << std::endl;

    move_player(game, Direction::LEFT, 1);
    move_player(game, Direction::RIGHT, 1);
    move_player(game, Direction::DOWN, 1);
    move_player(game, Direction::LEFT, 1);
    move_player(game, Direction::UP, 1);
    move_player(game, Direction::RIGHT, 1);
    move_player(game, Direction::RIGHT, 1);
    move_player(game, Direction::UP, 1);
    move_player(game, Direction::UP, 1);

    // shoot(game, Direction::RIGHT, 2);
    move_player(game, Direction::RIGHT, 2);

    // shoot(game, Direction::UP, 1);

    // Mostrar los objetos en la matriz
    std::vector<Object> objects = game.get_objects();
    std::cout << "Objetos al finalizar" << std::endl;
    for (const auto& obj: objects) {
        std::cout << "\tObjeto ID: " << obj.get_id()
                  << ", Tipo: " << game.get_object_type(static_cast<ObjectType>(obj.get_type()))
                  << ", Posición: (" << obj.get_position()[0] << ", " << obj.get_position()[1]
                  << ")" << std::endl;
    }

    return 0;
}
