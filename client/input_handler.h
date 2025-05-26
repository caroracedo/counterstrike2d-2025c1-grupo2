#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/action_DTO.h"
#include "../common/constants.h"

using namespace SDL2pp;

class InputHandler {
private:
    Renderer& renderer;
    Texture& cuerpo;
    Texture& piernas;

    float posX = 0.0f;
    float posY = 0.0f;

public:
    InputHandler(Renderer& renderer, Texture& cuerpo, Texture& piernas):
            renderer(renderer), cuerpo(cuerpo), piernas(piernas) {}

    ActionDTO receive_and_parse_action() const {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                return {ActionType::QUIT};
            }
        }
        const uint8_t* keystates = SDL_GetKeyboardState(nullptr);

        if (keystates[SDL_SCANCODE_W])
            return {ActionType::MOVE, Direction::UP};
        if (keystates[SDL_SCANCODE_S])
            return {ActionType::MOVE, Direction::DOWN};
        if (keystates[SDL_SCANCODE_A])
            return {ActionType::MOVE, Direction::LEFT};
        if (keystates[SDL_SCANCODE_D])
            return {ActionType::MOVE, Direction::RIGHT};

        return {};
    }

    // nose q mas
    bool update_graphics(const ActionDTO& action_update) {
        if (action_update.type == ActionType::MOVE) {
            posX = action_update.position[0];
            posY = action_update.position[1];
        }
        // Centro del jugador
        float centerX = posX + PLAYER_WIDTH / 2.0f;
        float centerY = posY + PLAYER_HEIGHT / 2.0f;

        // Mouse
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        float dx = mouseX - centerX;
        float dy = mouseY - centerY;
        float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;

        // Render
        renderer.SetDrawColor(255, 255, 255, 255);
        renderer.Clear();

        // Cuerpo rotado
        renderer.Copy(
                cuerpo, Rect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                Rect(static_cast<int>(posX), static_cast<int>(posY), PLAYER_WIDTH, PLAYER_HEIGHT),
                angle, SDL_Point{PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2}, SDL_FLIP_NONE);

        renderer.Present();
        SDL_Delay(1);
        return true;  // Por el momento...
    }
};

#endif  // INPUT_HANDLER_H
