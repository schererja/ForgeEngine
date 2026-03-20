#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]){
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow(
        "Forge Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Main Loop
    bool running = true;
    SDL_Event event;
    while (running) {
        //Handle Events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT){
                running = false;
            }
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                }
            }
        }
        // clear the screen to dark gray
        SDL_Surface* surface = SDL_GetWindowSurface(window);
        SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 50, 50, 50));
        SDL_UpdateWindowSurface(window);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
    
}