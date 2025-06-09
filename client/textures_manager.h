#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <memory>
#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

class TextureManager {
private:
    std::unordered_map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;

public:
    explicit TextureManager(SDL2pp::Renderer& renderer) {
        textures["terrorists"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/player/t1.bmp")));
        textures["counter_terrorists"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/player/ct1.bmp")));
        textures["legs"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../primer_fila_sin_padding.bmp")));
        textures["background"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../dustroof.bmp")));
        textures["box_texture"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../cuadro_fila5_columna3.bmp"))
                                  .SetBlendMode(SDL_BLENDMODE_BLEND));
        textures["box_texture_2"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../recorte_fila5-6_columna4-5.bmp"))
                                  .SetBlendMode(SDL_BLENDMODE_BLEND));
        textures["hud_numbers"] = std::make_shared<SDL2pp::Texture>(
                renderer,
                SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/hud_nums.bmp")).SetColorKey(true, 0));
        textures["hud_life"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../sprite_vida.bmp")).SetColorKey(true, 0));
        textures["hud_money"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../sprite_plata.bmp")).SetColorKey(true, 0));
        textures["hud_time"] = std::make_shared<SDL2pp::Texture>(
                renderer,
                SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/hud_nums.bmp")).SetColorKey(true, 0));
        // textures["background"] = std::make_shared<SDL2pp::Texture>(renderer,
        // SDL2pp::Surface(SDL_LoadBMP("../dustroof.bmp")));
        textures["glock"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/weapons/glock.bmp")));
        textures["ak-47"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/weapons/ak47.bmp")));
        textures["m3"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/weapons/m3.bmp")));
        textures["awp"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/weapons/awp.bmp")));
        textures["knife"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/weapons/knife.bmp")));
        textures["bomb"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/weapons/bomb.bmp")));
        textures["explotion"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../explosion.bmp")));
        textures["bomb_zone"] = std::make_shared<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../bomb_zone.bmp")));
    }

    std::shared_ptr<SDL2pp::Texture> get_texture(const std::string& texture_name) {
        return textures[texture_name];
    }
};

#endif
