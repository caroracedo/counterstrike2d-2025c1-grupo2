#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/action_DTO.h"
#include "../common/constants.h"

#include "game_camera.h"
#include "legs_view.h"
#include "player_view.h"
#include "sound_manager.h"

// #include "shop_view.h"

class InputHandler {
private:
    float fire_angle = 0.0f;
    bool fire_requested = false;
    GameCamera& camera;
    // ShopView& shop;
    // PlayerView& player;
    // LegsView& legs;

    SoundManager& sounds;


public:
    explicit InputHandler(GameCamera& cam, SoundManager& sound): camera(cam), sounds(sound) {}
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
                    case SDLK_w: {
                        // sounds.play("steps_1",0);
                        return {ActionType::MOVE, Direction::UP};
                    }

                    case SDLK_s: {
                        // sounds.play("steps_1",0);
                        return {ActionType::MOVE, Direction::DOWN};
                    }

                    case SDLK_a: {
                        // sounds.play("steps_1",0);
                        return {ActionType::MOVE, Direction::LEFT};
                    }

                    case SDLK_d: {
                        // sounds.play("steps_1",0);
                        return {ActionType::MOVE, Direction::RIGHT};
                    }

                    case SDLK_b:
                        return ActionDTO{ActionType::BOMB};
                    case SDLK_1:
                        return ActionDTO{ActionType::WEAPON, WeaponModel::AK47};
                    case SDLK_2:
                        return ActionDTO{ActionType::WEAPON, WeaponModel::M3};
                    case SDLK_3:
                        return ActionDTO{ActionType::WEAPON, WeaponModel::AWP};
                    case SDLK_4:
                        return ActionDTO{ActionType::AMMO, 30};
                    case SDLK_5:
                        return ActionDTO{ActionType::AMMO, 30};
                    case SDLK_SPACE:
                        return ActionDTO{ActionType::CHANGE};
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                uint16_t real_mouse_x = static_cast<uint16_t>(event.button.x) +
                                        static_cast<uint16_t>(camera.get_x());
                uint16_t real_mouse_y = static_cast<uint16_t>(event.button.y) +
                                        static_cast<uint16_t>(camera.get_y());
                return {ActionType::SHOOT, std::vector<uint16_t>{real_mouse_x, real_mouse_y}};
            }
        }
        return {};
    }

    bool is_fire_requested() const { return fire_requested; }

    float get_fire_angle() const { return fire_angle; }
};

#endif  // INPUT_HANDLER_H
