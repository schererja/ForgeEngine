#pragma once

#include <SDL2/SDL.h>

#include <string>

namespace Forge {

class Input;
// RAII wrapper for SDL window creation, OpenGL context, and event polling.
class Window {
   public:
    Window(const std::string& title, int width, int height);
    ~Window();

    // Rule of Five - delete copy constructor and copy assignment operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Moving is fine
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    // Interface
    bool isOpen() const;
    void swapBuffers();
    void pollEvents(Input* input = nullptr);

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    SDL_Window* getSDLWindow() const { return sdlWindow; }

   private:
    // Owned SDL resources.
    SDL_Window* sdlWindow = nullptr;
    SDL_GLContext glContext = nullptr;

    // Cached window state.
    int width = 0;
    int height = 0;
    bool open = false;
};

}  // namespace Forge