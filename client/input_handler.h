#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include "../common/action_DTO.h"
#include "../common/constants.h"

using namespace SDL2pp;

class InputHandler {

public:
    ActionDTO receive_and_parse_action() const {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                return {ActionType::QUIT};
            }
        }
        const uint8_t* keystates = SDL_GetKeyboardState(nullptr);

        if (keystates[SDL_SCANCODE_W]) return {ActionType::MOVE, Direction::UP};
        if (keystates[SDL_SCANCODE_S]) return {ActionType::MOVE, Direction::DOWN};
        if (keystates[SDL_SCANCODE_A]) return {ActionType::MOVE, Direction::LEFT};
        if (keystates[SDL_SCANCODE_D]) return {ActionType::MOVE, Direction::RIGHT};

        return {};
    }

};

#endif // INPUT_HANDLER_H
