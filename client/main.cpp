#include <exception>
#include <iostream>

// #include <SDL2/SDL.h>
// #include <SDL2pp/SDL2pp.hh>

#include "client.h"

// using SDL2pp;

#define EXPECTED_ARGC 3

int main(int argc, char* argv[]) {
    try {
        // // Initialize SDL library
        // SDL sdl(SDL_INIT_VIDEO);

        // // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
        // Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
        //               SDL_WINDOW_RESIZABLE);

        // // Create accelerated video renderer with default driver
        // Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        // // Clear screen
        // renderer.Clear();

        // // Show rendered frame
        // renderer.Present();

        // // 5 second delay
        // SDL_Delay(5000);

        // // Here all resources are automatically released and library deinitialized
        // return 0;

        // Momentáneamente agrego esto
        const char* hostname = NULL;
        const char* servname = NULL;

        if (argc != EXPECTED_ARGC) {
            std::cerr << "Bad program call. Expected: " << argv[0] << " <hostname> <servname>\n";
            return EXIT_FAILURE;
        }

        hostname = argv[1];
        servname = argv[2];

        Client client(hostname, servname);
        client.initiate_communication();
        return EXIT_SUCCESS;

    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
