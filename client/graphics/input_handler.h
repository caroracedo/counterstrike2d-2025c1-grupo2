#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "common/action_DTO.h"
#include "common/constants.h"

#include "game_view.h"

class InputHandler {
private:
    float last_angle_sent = 0.0f;

    const float angle_threshold = 2.0f;

    float last_mouse_x = 0.0f;
    float last_mouse_y = 0.0f;

    GameView& game_view;

    GameCamera& camera = game_view.get_camera();

    ShopView& shop = game_view.get_shop();


public:
    explicit InputHandler(GameView& game_view): game_view(game_view) {}

    std::vector<ActionDTO> receive_and_parse_actions() {
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
                        if (event.key.keysym.mod & KMOD_CTRL) {
                            actions.push_back(ActionDTO{ActionType::HEALTHCHEAT});
                        } else {
                            shop.handle_button_pressed(0);
                            actions.push_back(ActionDTO{ActionType::WEAPON, WeaponModel::AK47});
                        }
                        break;
                    case SDLK_2:
                        if (event.key.keysym.mod & KMOD_CTRL) {
                            actions.push_back(ActionDTO{ActionType::AMMOCHEAT});
                        } else {
                            shop.handle_button_pressed(1);
                            actions.push_back(ActionDTO{ActionType::WEAPON, WeaponModel::M3});
                        }
                        break;
                    case SDLK_3:
                        if (event.key.keysym.mod & KMOD_CTRL) {
                            actions.push_back(ActionDTO{ActionType::MONEYCHEAT});
                        } else {
                            shop.handle_button_pressed(2);
                            actions.push_back(ActionDTO{ActionType::WEAPON, WeaponModel::AWP});
                        }
                        break;
                    case SDLK_4:
                        if (event.key.keysym.mod & KMOD_CTRL) {
                            actions.push_back(ActionDTO{ActionType::WINCHEAT});
                        } else {
                            shop.handle_button_pressed(3);
                            actions.push_back(
                                    ActionDTO{ActionType::AMMOPRIMARY, static_cast<uint16_t>(30)});
                        }
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

                game_view.handle_attack();

                actions.push_back(ActionDTO{ActionType::SHOOT,
                                            std::vector<uint16_t>{real_mouse_x, real_mouse_y}});
            }

            auto player_pos = game_view.player_position();
            if (event.type == SDL_MOUSEMOTION && player_pos.size() == 2) {
                int dx = std::abs(event.motion.x - last_mouse_x);
                int dy = std::abs(event.motion.y - last_mouse_y);
                if (dx >= 3 || dy >= 3) {  // solo si se mueve al menos 3 píxeles
                    last_mouse_x = event.motion.x;
                    last_mouse_y = event.motion.y;
                    float angle = calculate_angle(player_pos);
                    if (std::abs(angle - last_angle_sent) > angle_threshold) {
                        actions.push_back(ActionDTO{ActionType::ROTATE, angle});
                        last_angle_sent = angle;
                    }
                }
            }
        }

        return actions;
    }


    float calculate_angle(const std::vector<uint16_t>& player_pos) {
        float screenX = static_cast<float>(player_pos[0]) - camera.get_x();
        float screenY = static_cast<float>(player_pos[1]) - camera.get_y();

        float centerX = screenX;
        float centerY = screenY;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        float dx = mouseX - centerX;
        float dy = mouseY - centerY;

        float angle = std::atan2(-dy, dx) * 180.0f / M_PI;

        if (angle < 0.0f)
            angle += 360.0f;

        return angle;
    }
};

#endif  // INPUT_HANDLER_H
