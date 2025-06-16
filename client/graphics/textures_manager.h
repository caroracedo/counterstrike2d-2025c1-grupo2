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
        load(renderer, "terrorists", ASSETS_PATH "/gfx/player/t1.bmp");
        load(renderer, "counter_terrorists", ASSETS_PATH "/gfx/player/ct1.bmp");
        load(renderer, "legs", ASSETS_PATH "/gfx/player/legs.bmp");
        load(renderer, "background", ASSETS_PATH "/gfx/backgrounds/dustroof.bmp");

        load(renderer, "hud_numbers", ASSETS_PATH "/gfx/hud_nums.bmp", true);
        load(renderer, "hud_life", ASSETS_PATH "/gfx/life.bmp", true);
        load(renderer, "hud_money", ASSETS_PATH "/gfx/money.bmp", true);

        load(renderer, "glock", ASSETS_PATH "/gfx/weapons/glock.bmp");
        load(renderer, "ak47", ASSETS_PATH "/gfx/weapons/ak47.bmp");
        load(renderer, "m3", ASSETS_PATH "/gfx/weapons/m3.bmp");
        load(renderer, "awp", ASSETS_PATH "/gfx/weapons/awp.bmp");
        load(renderer, "knife", ASSETS_PATH "/gfx/weapons/knife.bmp");
        load(renderer, "bomb", ASSETS_PATH "/gfx/weapons/bomb.bmp");
        load(renderer, "explotion", ASSETS_PATH "/gfx/sprites/explosion.bmp");
        load(renderer, "bomb_zone", ASSETS_PATH "/gfx/tiles/bomb_zone.bmp");
        load(renderer, "pointer", ASSETS_PATH "/gfx/player/pointer.bmp", true, {255, 0, 255, 0});

        load(renderer, "sand1", ASSETS_PATH "/gfx/backgrounds/sand1.bmp");
        load(renderer, "office", ASSETS_PATH "/gfx/backgrounds/office.bmp");
        load(renderer, "aztec", ASSETS_PATH "/gfx/backgrounds/aztec.bmp");


        for (int i = 1; i <= 5; ++i) {
            std::string name = "box_" + std::to_string(i);
            std::string path =
                    std::string(ASSETS_PATH) + "/gfx/tiles/caja" + std::to_string(i) + ".bmp";
            load(renderer, name, path);
        }
    }

    void load(SDL2pp::Renderer& renderer, const std::string& name, const std::string& path,
              bool colorkey = false, SDL_Color key_color = {0, 0, 0, 0}) {
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());
        if (!surface) {
            std::cerr << "[ERROR] Failed to load texture '" << name << "' from '" << path
                      << "': " << SDL_GetError() << std::endl;
            throw std::runtime_error("Texture load failed");
        }

        SDL2pp::Surface wrapped(surface);
        if (colorkey) {
            Uint32 key = SDL_MapRGB(wrapped.Get()->format, key_color.r, key_color.g, key_color.b);
            wrapped.SetColorKey(true, key);
        }

        textures[name] = std::make_shared<SDL2pp::Texture>(renderer, wrapped);
    }


    std::shared_ptr<SDL2pp::Texture> get_texture(const std::string& texture_name) {
        return textures[texture_name];
    }
};

#endif
