#ifndef FOV_VIEW_H
#define FOV_VIEW_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"

#include "textures_manager.h"

class FOVView {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
    bool visible = false;

public:
    FOVView(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void generate_FOV_texture(int width, int height, int circleRadius, int coneAngleDeg);
    void draw(float angle_deg);
    void set_visible(bool visible);
    bool is_visible() const;
};

#endif  // FOV_VIEW_H
