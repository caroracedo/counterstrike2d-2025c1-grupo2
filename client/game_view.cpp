#include "game_view.h"

#include <SDL2/SDL.h>

// de esto voy a hacer un refactor para que no se carguen las texturas en el constructor
// se haria una clase mas prolija.
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

    obstacles.clear();
    bullets.clear();
    for(const auto& object : action.objects) {

        if (object.type == ObjectType::PLAYER)
            update_player(object);
       
        // se encapsula en una clase
        else if (object.type == ObjectType::OBSTACLE) 
            update_obstacles(object);

        // else if (object.type == ObjectType::BULLET)
        //     update_bullets(object);
    }    
}

void GameView::update_player(const ObjectDTO& object){
    float x = object.position[0];
    float y = object.position[1];
    bool moved = (x != last_px || y != last_py);
    legs_view.update_position(x, y);
    std::cout << "Player moved to: " << x << ", " << y << std::endl;
    if (moved){
        legs_view.update_animation();

        player_view.update_position(x, y);

        last_px = x;
        last_py = y;

    }
}
void GameView::update_bullets(const ObjectDTO& object) {
    BulletView bullet(object.position[0], object.position[1]);
    // bullet.angle = object.angle; // Asumiendo que el DTO tiene un campo de ángulo
    bullets.push_back(bullet);
}
void GameView::update_obstacles(const ObjectDTO& object) {
    ObstacleView obs;
    obs.x = object.position[0];
    obs.y = object.position[1];
    obs.w = object.width;
    obs.h = object.height;
    obs.use_texture2 = (object.width == 64 && object.height == 64); // ejemplo de uso de textura2
    obstacles.push_back(obs);
}


void GameView::render() {
   
    float px = player_view.get_x();
    float py = player_view.get_y();
    camera.center_on(px + PLAYER_WIDTH / 2, 
                 py + PLAYER_HEIGHT / 2);


    renderer.SetDrawColor(255, 255, 255, 255);
    renderer.Clear();

    // Rect view = camera.get_viewport();

    renderer.Copy(background, Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));      

    legs_view.draw(renderer,camera);
    
    for (auto& bullet : bullets)
        bullet.draw(renderer, camera);

    player_view.draw(renderer,camera);

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
