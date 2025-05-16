#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << "\n";
        return 1;
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        std::cerr << "Error al inicializar SDL_image: " << IMG_GetError() << "\n";
        return 1;
    }

    SDL_Surface* original = IMG_Load("assets/gfx/player/ct1.bmp");
    if (!original) {
        std::cerr << "Error cargando imagen: " << IMG_GetError() << "\n";
        return 1;
    }

    // Supongamos que la imagen tiene 2 columnas x 3 filas = 6 sprites
    int spriteWidth = original->w / 2;
    int spriteHeight = original->h / 3;

    // Creamos la nueva superficie: 6 sprites en línea horizontal
    SDL_Surface* sheet = SDL_CreateRGBSurface(0, spriteWidth * 6, spriteHeight, 
                                              original->format->BitsPerPixel,
                                              original->format->Rmask,
                                              original->format->Gmask,
                                              original->format->Bmask,
                                              original->format->Amask);

    if (!sheet) {
        std::cerr << "Error creando superficie: " << SDL_GetError() << "\n";
        return 1;
    }

    // Copiamos cada sprite a la nueva superficie en horizontal
    int index = 0;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 2; ++col) {
            SDL_Rect srcRect = { col * spriteWidth, row * spriteHeight, spriteWidth, spriteHeight };
            SDL_Rect destRect = { index * spriteWidth, 0, spriteWidth, spriteHeight };
            SDL_BlitSurface(original, &srcRect, sheet, &destRect);
            ++index;
        }
    }

    // Guardamos la nueva imagen en formato .bmp
    if (SDL_SaveBMP(sheet, "sprite_sheet.bmp") != 0) {
        std::cerr << "Error guardando BMP: " << SDL_GetError() << "\n";
    } else {
        std::cout << "Sprite sheet guardado como sprite_sheet.bmp\n";
    }

    SDL_FreeSurface(original);
    SDL_FreeSurface(sheet);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
