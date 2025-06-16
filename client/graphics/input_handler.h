#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "common/action_DTO.h"
#include "common/constants.h"

#include "game_camera.h"
#include "legs_view.h"
#include "player_view.h"
#include "shop_view.h"
#include "sound_manager.h"

class InputHandler {
private:
    float last_angle_sent = 0.0f;
    bool fire_requested = false;
    GameCamera& camera;
    ShopView& shop;


public:
    explicit InputHandler(GameCamera& cam, ShopView& shop): camera(cam), shop(shop) {}
    std::vector<ActionDTO> receive_and_parse_actions(const std::vector<uint16_t>& player_pos) {
        fire_requested = false;
        SDL_Event event;
        std::vector<ActionDTO> actions;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                actions.push_back(ActionDTO{ActionType::QUIT});
                break;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        actions.push_back(ActionDTO{ActionType::MOVE, Direction::UP});
                        break;
                    case SDLK_s:
                        actions.push_back(ActionDTO{ActionType::MOVE, Direction::DOWN});
                        break;
                    case SDLK_a:
                        actions.push_back(ActionDTO{ActionType::MOVE, Direction::LEFT});
                        break;
                    case SDLK_d:
                        actions.push_back(ActionDTO{ActionType::MOVE, Direction::RIGHT});
                        break;
                    case SDLK_b:
                        actions.push_back(ActionDTO{ActionType::BOMB});
                        break;
                    case SDLK_1:
                        shop.handle_button_pressed(0);
                        actions.push_back(ActionDTO{ActionType::WEAPON, WeaponModel::AK47});
                        break;
                    case SDLK_2:
                        shop.handle_button_pressed(1);
                        actions.push_back(ActionDTO{ActionType::WEAPON, WeaponModel::M3});
                        break;
                    case SDLK_3:
                        shop.handle_button_pressed(2);
                        actions.push_back(ActionDTO{ActionType::WEAPON, WeaponModel::AWP});
                        break;
                    case SDLK_4:
                        shop.handle_button_pressed(3);
                        actions.push_back(
                                ActionDTO{ActionType::AMMOPRIMARY, static_cast<uint16_t>(30)});
                        break;
                    case SDLK_5:
                        shop.handle_button_pressed(4);
                        actions.push_back(
                                ActionDTO{ActionType::AMMOSECONDARY, static_cast<uint16_t>(30)});
                        break;
                    case SDLK_SPACE:
                        actions.push_back(ActionDTO{ActionType::CHANGE});
                        break;
                    case SDLK_r:
                        actions.push_back(ActionDTO{ActionType::PICKUP});
                        break;
                    case SDLK_RETURN:
                        actions.push_back(ActionDTO{ActionType::START});
                        break;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                uint16_t real_mouse_x = static_cast<uint16_t>(event.button.x) +
                                        static_cast<uint16_t>(camera.get_x());
                uint16_t real_mouse_y = static_cast<uint16_t>(event.button.y) +
                                        static_cast<uint16_t>(camera.get_y());
                actions.push_back(ActionDTO{ActionType::SHOOT,
                                            std::vector<uint16_t>{real_mouse_x, real_mouse_y}});
            }

            if (event.type == SDL_MOUSEMOTION && player_pos.size() == 2) {
                float angle = calculate_angle(player_pos);
                actions.push_back(ActionDTO{ActionType::ROTATE, (_Float16)angle});
            }
        }

        return actions;
    }


    float calculate_angle(const std::vector<uint16_t>& player_pos) {

        float screenX = player_pos[0] - camera.get_x();
        float screenY = player_pos[1] - camera.get_y();

        float centerX = screenX + PLAYER_WIDTH / 2.0f;
        float centerY = screenY + PLAYER_HEIGHT / 2.0f;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        float dx = mouseX - centerX;
        float dy = mouseY - centerY;

        float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;

        return angle;
    }

    bool is_fire_requested() const { return fire_requested; }

    // float get_fire_angle() const { return fire_angle; }
};

#endif  // INPUT_HANDLER_H
