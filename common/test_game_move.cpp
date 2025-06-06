#include <iostream>

#include "action_DTO.h"
#include "config.h"
#include "game.h"

#define MOVE_INPUT "mover"
#define SHOOT_INPUT "disparar"
#define BOMB_INPUT "bomba"
#define QUIT_INPUT "q"
#define W_INPUT "w"
#define A_INPUT "a"
#define S_INPUT "s"
#define D_INPUT "d"

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

ActionDTO receive_and_parse_action() {
    std::string input;
    std::getline(std::cin, input);
    std::istringstream iss(input);

    std::string action_input;
    if (!(iss >> action_input))
        return {};

    if (action_input == QUIT_INPUT) {
        return ActionDTO(ActionType::QUIT);
    } else if (action_input == MOVE_INPUT) {
        std::string direction_input;
        if (!(iss >> direction_input))
            return {};
        Direction direction;
        if (direction_input == W_INPUT)
            direction = Direction::UP;
        else if (direction_input == A_INPUT)
            direction = Direction::LEFT;
        else if (direction_input == S_INPUT)
            direction = Direction::DOWN;
        else if (direction_input == D_INPUT)
            direction = Direction::RIGHT;
        else
            return {};

        return {ActionType::MOVE, direction};
    } else if (action_input == SHOOT_INPUT) {
        std::vector<uint16_t> desired_position(2);
        if (!(iss >> desired_position[0] >> desired_position[1]))
            return {};
        return {ActionType::SHOOT, desired_position};
    } else if (action_input == BOMB_INPUT) {
        return ActionDTO(ActionType::BOMB);
    }
    return {};
}

int main() {
    Config config("config/config.yaml");
    Game game(config);

    game.add_player(PlayerType::TERRORIST, 1);
    game.add_player(PlayerType::COUNTERTERRORIST, 2);
    show(game);

    std::vector<uint16_t> player2_position = game.get_player_position(2);

    while (true) {
        ActionDTO action = receive_and_parse_action();
        switch (action.type) {
            case ActionType::MOVE:
                game.move(action.direction, 1);
                break;
            case ActionType::SHOOT:
                game.shoot(player2_position, 1);
                break;
            case ActionType::QUIT:
                return 0;
            default:
                std::cout << "\tAccion desconocida" << std::endl;
                break;
        }
        show(game);
    }

    return 0;
}
