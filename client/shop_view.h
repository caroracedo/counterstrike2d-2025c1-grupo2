#ifndef BUY_MENU_VIEW_H
#define BUY_MENU_VIEW_H

#include <string>
#include <vector>

#include <SDL2pp/Font.hh>
#include <SDL2pp/SDL2pp.hh>

#include "../common/constants.h"

#define SCREEN_MARGIN 20


struct Button {
    SDL2pp::Rect rect;
    std::string text;
    bool visible = false;
    bool pressed = false;
    Uint32 press_time;
};

class ShopView {
private:
    SDL2pp::SDLTTF ttf;

    SDL2pp::Renderer& renderer;

    std::vector<Button> buttons;

    SDL2pp::Rect overlay_rect;

    SDL2pp::Font font;

    bool visible = true;

    void init_buttons();

    void init_resources();

public:
    explicit ShopView(SDL2pp::Renderer& renderer);

    void render();

    void set_visible(bool visible);

    bool is_visible() const;

    void handle_button_pressed(int index_button);
};

#endif  // BUY_MENU_VIEW_H
