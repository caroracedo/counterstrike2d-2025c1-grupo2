#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <chrono>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <thread>

#include "../common/action_DTO.h"

#define MOVE_INPUT "mover"
#define QUIT_INPUT "q"
#define W_INPUT "w"
#define A_INPUT "a"
#define S_INPUT "s"
#define D_INPUT "d"

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

        if (action_input == MOVE_INPUT) {
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
        }
        return {};
    }

    bool update_graphics(const ActionDTO& action_update) {
        if (action_update.type == ActionType::MOVE) {
            std::cout << '(' << static_cast<unsigned int>(action_update.position[0]) << ','
                      << static_cast<unsigned int>(action_update.position[1]) << ')' << std::endl;
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // Simular delay
        return false;
    }
};

#endif  // INPUT_PARSER_H
