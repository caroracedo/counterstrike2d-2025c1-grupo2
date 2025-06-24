#include "FOV_view.h"

FOVView::FOVView(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        renderer(renderer), texture_manager(texture_manager) {}

void FOVView::generate_FOV_texture(int width, int height, int circleRadius, int coneAngleDeg) {
    SDL2pp::Texture texture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width,
                            height);

    SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer.Get());
    SDL_SetRenderTarget(renderer.Get(), texture.Get());

    SDL_SetRenderDrawColor(renderer.Get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.Get());

    int cx = width / 2;
    int cy = height / 2;

    filledCircleRGBA(renderer.Get(), cx, cy, circleRadius, 255, 255, 255, 255);

    double angle_rad = coneAngleDeg * M_PI / 180.0;
    double half_angle = angle_rad / 2.0;
    double len = std::sqrt(width * width + height * height);

    int x1 = cx, y1 = cy;
    int x2 = static_cast<int>(cx + len * cos(-half_angle));
    int y2 = static_cast<int>(cy + len * sin(-half_angle));
    int x3 = static_cast<int>(cx + len * cos(half_angle));
    int y3 = static_cast<int>(cy + len * sin(half_angle));

    filledTrigonRGBA(renderer.Get(), x1, y1, x2, y2, x3, y3, 255, 255, 255, 255);

    SDL_Surface* surface =
            SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) {
        SDL_SetRenderTarget(renderer.Get(), prevTarget);
        throw std::runtime_error("Error creando la superficie para guardar BMP");
    }

    if (SDL_RenderReadPixels(renderer.Get(), nullptr, SDL_PIXELFORMAT_RGBA8888, surface->pixels,
                             surface->pitch) != 0) {
        SDL_FreeSurface(surface);
        SDL_SetRenderTarget(renderer.Get(), prevTarget);
        throw std::runtime_error(std::string("Error leyendo pixels del render target: ") +
                                 SDL_GetError());
    }

    if (SDL_SaveBMP(surface, ASSETS_PATH "/gfx/stencil_fov.bmp") != 0) {
        SDL_FreeSurface(surface);
        SDL_SetRenderTarget(renderer.Get(), prevTarget);
        throw std::runtime_error(std::string("Error guardando BMP: ") + SDL_GetError());
    }

    SDL_FreeSurface(surface);

    SDL_SetRenderTarget(renderer.Get(), prevTarget);

    texture_manager.load(renderer, "stencil_fov", ASSETS_PATH "/gfx/stencil_fov.bmp", true,
                         {255, 255, 255, 0});
}

void FOVView::draw(float angle_deg) {
    auto tex_ptr = texture_manager.get_texture("stencil_fov");
    if (!tex_ptr)
        return;

    int w = tex_ptr->GetWidth();
    int h = tex_ptr->GetHeight();
    tex_ptr->SetBlendMode(SDL_BLENDMODE_BLEND);
    tex_ptr->SetAlphaMod(FOV_OPACITY);
    SDL2pp::Rect dst = {SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h};
    SDL2pp::Rect src = {0, 0, w, h};
    SDL_Point center = {w / 2, h / 2};

    renderer.Copy(*tex_ptr, src, dst, angle_deg - 90.0f, center, SDL_FLIP_NONE);
}

void FOVView::set_visible(bool visible) { this->visible = visible; }

bool FOVView::is_visible() const { return visible; }
