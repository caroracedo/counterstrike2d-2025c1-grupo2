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
#include "shop_view.h"
#include "sound_manager.h"

class InputHandler {
private:
    float fire_angle = 0.0f;
    bool fire_requested = false;
    GameCamera& camera;
    ShopView& shop;


public:
    explicit InputHandler(GameCamera& cam, ShopView& shop): camera(cam), shop(shop) {}
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
                        return ActionDTO{ActionType::BOMB};

                    case SDLK_1: {
                        shop.handle_button_pressed(0);
                        return ActionDTO{ActionType::WEAPON, WeaponModel::AK47};
                    }

                    case SDLK_2: {
                        shop.handle_button_pressed(1);
                        return ActionDTO{ActionType::WEAPON, WeaponModel::M3};
                    }

                    case SDLK_3: {
                        shop.handle_button_pressed(2);
                        return ActionDTO{ActionType::WEAPON, WeaponModel::AWP};
                    }

                    case SDLK_4: {
                        shop.handle_button_pressed(3);
                        return ActionDTO{ActionType::AMMOPRIMARY, 30};
                    }

                    case SDLK_5: {
                        shop.handle_button_pressed(4);
                        return ActionDTO{ActionType::AMMOSECONDARY, 30};
                    }

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
