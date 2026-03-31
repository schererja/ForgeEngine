#include "Window.h"

#include <SDL2/SDL.h>
#include <glad/gl.h>

#include <iostream>

#include "Input.h"
#include "Log.h"
namespace Forge {

Window::Window(const std::string& title, int width, int height)
    : width(width), height(height) {
    // Initialize SDL video subsystem before any window/context creation.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        FORGE_ERROR("SDL_Init Error: " + std::string(SDL_GetError()));
        return;
    }

    // Request an OpenGL 3.3 core profile context.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create SDL window with OpenGL support.
    sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, width, height,
                                 SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (!sdlWindow) {
        FORGE_ERROR("SDL_CreateWindow Error: " + std::string(SDL_GetError()));

        return;
    }

    // Create OpenGL context bound to this window.
    glContext = SDL_GL_CreateContext(sdlWindow);
    if (!glContext) {
        FORGE_ERROR("SDL_GL_CreateContext Error: " +
                    std::string(SDL_GetError()));

        SDL_DestroyWindow(sdlWindow);
        sdlWindow = nullptr;
        return;
    }

    // Initialize GLEW after a valid OpenGL context is active.
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        FORGE_ERROR("GLAD initialization failed");
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        return;
    }

    // Enable V-Sync to cap frame rate to display refresh.
    SDL_GL_SetSwapInterval(1);

    // Log active graphics runtime versions for diagnostics.
    FORGE_INFO("OpenGL Version: " +
               std::string((const char*)glGetString(GL_VERSION)));
    FORGE_INFO("GLAD initialized successfully");

    open = true;
}

Window::~Window() {
    if (glContext) {
        SDL_GL_DeleteContext(glContext);
    }
    if (sdlWindow) {
        SDL_DestroyWindow(sdlWindow);
    }
    SDL_Quit();
}

Window::Window(Window&& other) noexcept {
    sdlWindow = other.sdlWindow;
    glContext = other.glContext;
    width = other.width;
    height = other.height;
    open = other.open;

    // Leave moved-from object inert.
    other.sdlWindow = nullptr;
    other.glContext = nullptr;
    other.open = false;
}

Window& Window::operator=(Window&& other) noexcept {
    if (this != &other) {
        // Release current resources before taking ownership of other's.
        if (glContext) {
            SDL_GL_DeleteContext(glContext);
        }
        if (sdlWindow) {
            SDL_DestroyWindow(sdlWindow);
        }

        sdlWindow = other.sdlWindow;
        glContext = other.glContext;
        width = other.width;
        height = other.height;
        open = other.open;

        other.sdlWindow = nullptr;
        other.glContext = nullptr;
        other.open = false;
    }
    return *this;
}

bool Window::isOpen() const { return open; }

void Window::swapBuffers() {
    if (sdlWindow) {
        SDL_GL_SwapWindow(sdlWindow);
    }
}

void Window::pollEvents(Input* input) {
    if (input) {
        input->beginFrame();
    }
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
                                       event.key.keysym.sym == SDLK_ESCAPE)) {
            open = false;
        }
        if (input) {
            input->processEvent(event);
        }
    }
}
}  // namespace Forge