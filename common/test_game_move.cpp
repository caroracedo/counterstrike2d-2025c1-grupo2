#include <iostream>

#include "config.h"
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
        for (auto v: game.get_player_position(id)) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "\tNo se pudo mover al jugador " << static_cast<int>(id) << " hacia "
                  << direction_to_string(direction) << std::endl;
    }
}

void shoot(Game& game, Direction direction, uint16_t id) {
    std::vector<uint16_t> position = game.get_player_position(id);
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
    Config config("config/config.yaml");
    Game game(config);

    game.add_player(PlayerType::TERRORIST, 1);
    game.add_player(PlayerType::COUNTERTERRORIST, 2);
    show(game);

    std::vector<uint16_t> player2_position = game.get_player_position(2);

    for (int j = 0; j < 100; ++j) {
        game.shoot(player2_position, 1);
        for (int i = 0; i < 20; ++i) {
            game.update();
        }
    }
    return 0;
}
