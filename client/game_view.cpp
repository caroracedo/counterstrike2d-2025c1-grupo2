#include "game_view.h"

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

GameView::GameView():  
    sdl(SDL_INIT_VIDEO),
    window("CS2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
    renderer(window, -1, SDL_RENDERER_ACCELERATED),
    body_sprites(renderer, Surface(SDL_LoadBMP("../assets/gfx/player/ct1.bmp"))),
    legs_sprites(renderer, Surface(SDL_LoadBMP("../primer_fila_sin_padding.bmp"))),
    player_view(body_sprites),
    legs_view(legs_sprites,
                {
                    Rect(0,  0, 32, 32),   
                    Rect(32, 0, 32, 32),   
                    Rect(64, 0, 32, 32)    
                },
                100),
    background (renderer, Surface(SDL_LoadBMP("../dustroof.bmp"))),
    camera(SCREEN_WIDTH, SCREEN_HEIGHT, 2048, 2048),
    // gun_texture(renderer, Surface(SDL_LoadBMP("../assets/gfx/weapons/ak47.bmp"))),
    // gun_view(gun_texture) 
    box_texture(renderer, Surface(SDL_LoadBMP("../cuadro_fila5_columna3.bmp"))),
    box_texture2(renderer, Surface(SDL_LoadBMP("../recorte_fila5-6_columna4-5.bmp")))
    {}


void GameView::update(const ActionDTO& action) {
   
    if (action.type != ActionType::UPDATE)
        return;

    obstacles.clear(); // Limpiar obstáculos antes de procesar nuevos objetos
    for(const auto& object : action.objects) {
        float x = object.position[0];
        float y = object.position[1];
        if (object.type == ObjectType::PLAYER) {
            bool moved = (x != last_px || y != last_py);

            legs_view.update_position(x, y);
            if (moved)
                legs_view.update_animation();

            player_view.update_position(x, y);

            last_px = x;
            last_py = y;
        }

        else if (object.type == ObjectType::OBSTACLE) {
            float w = object.width; 
            float h = object.height;
        
            ObstacleView obs;
            obs.x = x;
            obs.y = y;
            obs.w = w;
            obs.h = h;
            obs.use_texture2 = (w == 64 && h == 64); // Asumiendo que el tamaño 64x64 usa la segunda textura
            obstacles.push_back(obs);
            continue;
        }
    }    
}

void GameView::render() {
   
    float px = player_view.get_x();
    float py = player_view.get_y();
    camera.center_on(px + PLAYER_WIDTH / 2, 
                 py + PLAYER_HEIGHT / 2);


    renderer.SetDrawColor(255, 255, 255, 255);
    renderer.Clear();

    Rect view = camera.get_viewport();

    renderer.Copy(background,view, Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));      

    // SDL_Rect rojo = { 50 - camera.get_x(), 10 - camera.get_y(), 50, 50};
    // renderer.SetDrawColor( 255, 0, 0, 255);
    // renderer.FillRect(rojo);

    legs_view.draw(renderer,camera);
    
    // for (auto& bullet : bullets)
    //     bullet.draw(renderer, camera);

    player_view.draw(renderer,camera);
    // float angle_deg = player_view.get_angle();
    // gun_view.draw(renderer, camera, px, py, angle_deg);

    for (const auto& obs : obstacles) {
        SDL_Rect dst_rect = { int(obs.x) - camera.get_x(), int(obs.y) - camera.get_y(), int(obs.w), int(obs.h) };
        SDL_Rect src_rect = { 0, 0, int(obs.w), int(obs.h) }; // O el tamaño real de la textura
        if (obs.use_texture2) {
            renderer.Copy(box_texture2, src_rect, dst_rect);
        } else {
            renderer.Copy(box_texture, src_rect, dst_rect);
        }
    }
    
    renderer.Present();

    // SDL_Delay(16);  // TODO: hacer calculo de FPS no hardcodeado.
    
}

void GameView::update_graphics(const ActionDTO& action){
    update(action);
    render();
}

void GameView::frame_sync() {
    Uint32 now = SDL_GetTicks();
    Uint32 elapsed = now - last_frame_time;

    if (elapsed < frame_delay)
        SDL_Delay(frame_delay - elapsed);

    last_frame_time = SDL_GetTicks();
}
