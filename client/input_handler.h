#ifndef SDL_INPUT_PARSER_H
#define SDL_INPUT_PARSER_H

#include <SDL2/SDL.h>
#include "../common/action_DTO.h"

class InputHandler{ //por como esta definido ahora es una clase estática...
public:
    ActionDTO receive_and_parse_action() const {
        const uint8_t* keystates = SDL_GetKeyboardState(nullptr);

        if (keystates[SDL_SCANCODE_W]) return {ActionType::MOVE, Direction::UP};
        if (keystates[SDL_SCANCODE_S]) return {ActionType::MOVE, Direction::DOWN};
        if (keystates[SDL_SCANCODE_A]) return {ActionType::MOVE, Direction::LEFT};
        if (keystates[SDL_SCANCODE_D]) return {ActionType::MOVE, Direction::RIGHT};

        return {};
    }


    //nose q mas
};

#endif
