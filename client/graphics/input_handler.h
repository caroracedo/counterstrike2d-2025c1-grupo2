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
    GameCamera& camera;
    ShopView& shop;

public:
    explicit InputHandler(GameView& game_view);

    std::vector<ActionDTO> poll_actions();

    float calculate_angle(const std::vector<float>& player_pos);
};

#endif  // INPUT_HANDLER_H
