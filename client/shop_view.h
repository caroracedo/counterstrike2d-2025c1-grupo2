#ifndef BUY_MENU_VIEW_H
#define BUY_MENU_VIEW_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Font.hh>
#include <string>
#include <vector>

#include "../common/constants.h"

#define SCREEN_MARGIN 20


struct Button {
    SDL2pp::Rect rect;    
    std::string text;
    bool visible = false;

};

class ShopView {
private:
    SDL2pp::SDLTTF ttf;

    SDL2pp::Renderer& renderer;
    
    std::vector<Button> buttons;
    
    SDL2pp::Rect overlay_rect;
    
    SDL2pp::Font font;


    bool visible = true;
     

    void init_buttons() {
        int x = overlay_rect.x + 30;
        int y = overlay_rect.y + 50;
        int button_width = 200;
        int button_height = 35;
        int spacing = 30;

        std::vector<std::string> texts = {
            "1 AK-47",
            "2 M3",
            "3 AWP",
            "4 Primary Ammo",
            "5 Secondary Ammo",
            "0 Close"
        };

        for (size_t i = 0; i < texts.size(); ++i) {
            SDL2pp::Rect rect(x, y + static_cast<int>(i) * (button_height + spacing), button_width, button_height);
            buttons.push_back({rect, texts[i], true});
        }
    }

public:
    ShopView(SDL2pp::Renderer& ren ):
        renderer(ren),
        overlay_rect(SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_WIDTH - (SCREEN_MARGIN * 2), SCREEN_HEIGHT - (SCREEN_MARGIN * 2)),
        font("../assets/gfx/fonts/sourcesans.ttf",16)
    {init_buttons();}

    void render() {
        if (!visible) return;  // Si no es visible, no hacemos nada

        // Guardamos el color actual del renderer
        SDL2pp::Color prevColor = renderer.GetDrawColor();

        // Establecemos color gris semitransparente
        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(17, 17, 17, 200);


        renderer.FillRect(overlay_rect);

        // por las dudas...
        renderer.SetDrawColor(prevColor);

        for (const Button& btn : buttons) {
            if (!btn.visible) continue;

            renderer.SetDrawColor(17, 17, 17, 235); // gris oscuro
            renderer.FillRect(btn.rect);
            
            renderer.SetDrawColor(255, 255, 255, 235); // borde blanco
            renderer.DrawRect(btn.rect);

            // Renderizado del texto
            SDL2pp::Texture text_sprite(
				renderer,
				font.RenderText_Blended(btn.text, SDL_Color{255, 255, 255, 255})
			);
            // Centrado en el botón
            int text_x = btn.rect.x + (btn.rect.w - text_sprite.GetWidth()) / 2;
            int text_y = btn.rect.y + (btn.rect.h - text_sprite.GetHeight()) / 2;

            renderer.Copy(text_sprite, SDL2pp::Rect(text_x, text_y, text_sprite.GetWidth(), text_sprite.GetHeight()));
            // A futuro: acá podés renderizar el texto con SDL2pp::Font
        }
    }

    void set_visible(bool visible){
        this->visible = visible;
    }

    bool is_visible() const {
        return visible;
    }

};

#endif  // BUY_MENU_VIEW_H
