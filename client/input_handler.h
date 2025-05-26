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
                return {ActionType::QUIT};
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                int center_x = SCREEN_WIDTH / 2;   // O pasalo como parámetro
                int center_y = SCREEN_HEIGHT / 2;

                float dx = mouse_x - center_x;
                float dy = mouse_y - center_y;
                fire_angle = std::atan2(dy, dx) * 180.0f / M_PI;
                fire_requested = true;
            }
        }

        const uint8_t* keystates = SDL_GetKeyboardState(nullptr);
        if (keystates[SDL_SCANCODE_W]) return {ActionType::MOVE, Direction::UP};
        if (keystates[SDL_SCANCODE_S]) return {ActionType::MOVE, Direction::DOWN};
        if (keystates[SDL_SCANCODE_A]) return {ActionType::MOVE, Direction::LEFT};
        if (keystates[SDL_SCANCODE_D]) return {ActionType::MOVE, Direction::RIGHT};

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
