#include "shop_view.h"


ShopView::ShopView(SDL2pp::Renderer& renderer):
        renderer(renderer),
        overlay_rect(SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_WIDTH - (SCREEN_MARGIN * 2),
                     SCREEN_HEIGHT - (SCREEN_MARGIN * 2)),
        font(ASSETS_PATH "/gfx/fonts/korean.ttf", 16) {
    init_buttons();
}

void ShopView::init_buttons() {
    int x = overlay_rect.x + 30;
    int y = overlay_rect.y + 50;
    int button_width = 200;
    int button_height = 35;
    int spacing = 30;

    std::vector<std::string> texts = {"1 AK-47", "2 M3", "3 AWP", "4 Primary Ammo",
                                      "5 Secondary Ammo"};

    for (size_t i = 0; i < texts.size(); ++i) {
        SDL2pp::Rect rect(x, y + static_cast<int>(i) * (button_height + spacing), button_width,
                          button_height);
        buttons.push_back({rect, texts[i], true, false, 0});
    }
}

void ShopView::init_resources() {
    SDL2pp::Texture title_texture(renderer,
                                  font.RenderText_Blended("Buy", SDL_Color{255, 255, 255, 255}));

    SDL2pp::Point title_pos(overlay_rect.x + (overlay_rect.w - title_texture.GetWidth()) / 2,
                            overlay_rect.y + 10);

    renderer.Copy(title_texture, SDL2pp::NullOpt, SDL2pp::Rect(title_pos, title_texture.GetSize()));
}
void ShopView::render() {
    if (!visible)
        return;  // Si no es visible, no hacemos nada
    // Guardamos el color actual del renderer
    SDL2pp::Color prevColor = renderer.GetDrawColor();

    // Establecemos color gris semitransparente
    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(17, 17, 17, 200);


    renderer.FillRect(overlay_rect);


    // por las dudas...
    renderer.SetDrawColor(prevColor);

    init_resources();
    for (Button& btn: buttons) {
        if (!btn.visible)
            continue;

        Uint32 now = SDL_GetTicks();
        bool show_pressed = btn.pressed && (now - btn.press_time < 200);

        if (show_pressed) {
            renderer.SetDrawColor(118, 118, 118, 235);
        } else {
            renderer.SetDrawColor(17, 17, 17, 235);  // gris oscuro
            btn.pressed = false;
        }

        renderer.FillRect(btn.rect);

        renderer.SetDrawColor(255, 255, 255, 235);  // borde blanco
        renderer.DrawRect(btn.rect);

        SDL2pp::Texture text_sprite(
                renderer, font.RenderText_Blended(btn.text, SDL_Color{255, 255, 255, 255}));

        SDL2pp::Point text_pos(btn.rect.x + (btn.rect.w - text_sprite.GetWidth()) / 2,
                               btn.rect.y + (btn.rect.h - text_sprite.GetHeight()) / 2);

        renderer.Copy(text_sprite, SDL2pp::NullOpt, SDL2pp::Rect(text_pos, text_sprite.GetSize()));
    }
}

void ShopView::set_visible(bool visible) { this->visible = visible; }

bool ShopView::is_visible() const { return visible; }

void ShopView::handle_button_pressed(int index_button) {
    if (!visible || index_button < 0 || index_button >= static_cast<int>(buttons.size()))
        return;

    Button& btn = buttons[index_button];

    if (!btn.visible)
        return;

    btn.visible = true;

    btn.pressed = true;

    btn.press_time = SDL_GetTicks();

    // sonidos ...
}
