#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/action_DTO.h"
#include "../common/constants.h"

using namespace SDL2pp;

class InputHandler {
private:
    float fire_angle = 0.0f;
    bool fire_requested = false;

    public:
    ActionDTO receive_and_parse_action() {
        fire_requested = false;  // Reset
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                return ActionDTO{ActionType::QUIT};
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        return {ActionType::MOVE, Direction::UP};
                    case SDLK_s:
                        return {ActionType::MOVE, Direction::DOWN};
                    case SDLK_a:
                        return {ActionType::MOVE, Direction::LEFT};
                    case SDLK_d:
                        return {ActionType::MOVE, Direction::RIGHT};
                    case SDLK_b:
                        return {ActionType::BOMB};
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                std::cout << "Mouse button pressed at: (" << event.button.x << ", " << event.button.y << ")\n";
                std::cout << "Mouse button pressed at: (" << static_cast<uint16_t>(event.button.x) << ", " << static_cast<uint16_t>(event.button.y) << ")\n";
                return {ActionType::SHOOT, std::vector<uint16_t>{static_cast<uint16_t>(event.button.x), static_cast<uint16_t>(event.button.y)}};
            }
        }
        return {};
    }

    bool is_fire_requested() const {
        return fire_requested;
    }

    float get_fire_angle() const {
        return fire_angle;
    }
};

#endif  // INPUT_HANDLER_H
