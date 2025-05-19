#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <iostream>
#include <istream>
#include <sstream>
#include <string>

#include "../common/action_DTO.h"

#define MOVE_INPUT "mover"
#define SHOOT_INPUT "disparar"
#define QUIT_INPUT "q"
#define W_INPUT 'w'
#define A_INPUT 'a'
#define S_INPUT 's'
#define D_INPUT 'd'

class MockHandler {
public:
    /* Procesar */
    ActionDTO receive_and_parse_action() {
        std::string input;
        std::getline(std::cin, input);
        std::istringstream iss(input);

        std::string action_input;
        if (!(iss >> action_input))
            return {};

        if (action_input == QUIT_INPUT) {
            return {ActionType::QUIT};
        }

        if (action_input == MOVE_INPUT || action_input == SHOOT_INPUT) {
            std::string direction_input;
            if (!(iss >> direction_input) || direction_input.size() != 1)
                return {};

            Direction direction;
            switch (direction_input[0]) {
                case W_INPUT:
                    direction = Direction::UP;
                    break;
                case A_INPUT:
                    direction = Direction::LEFT;
                    break;
                case S_INPUT:
                    direction = Direction::DOWN;
                    break;
                case D_INPUT:
                    direction = Direction::RIGHT;
                    break;
                default:
                    return {};
            }

            if (action_input == MOVE_INPUT)
                return {ActionType::MOVE, direction};
            else
                return {direction, {WeaponType::GLOCK}};
        }
        return {};
    }

    bool update_graphics(const ActionDTO& action_update) {
        if (action_update.type == ActionType::MOVE) {
            std::cout << '(' << static_cast<unsigned int>(action_update.position[0]) << ','
                      << static_cast<unsigned int>(action_update.position[1]) << ')' << std::endl;
            return true;
        }
        if (action_update.type == ActionType::SHOOT) {
            std::cout << "Bala en posición: ("
                      << static_cast<unsigned int>(action_update.position[0]) << ','
                      << static_cast<unsigned int>(action_update.position[1]) << ')' << std::endl;
            return true;
        }
        return false;
    }
};

#endif  // INPUT_PARSER_H
