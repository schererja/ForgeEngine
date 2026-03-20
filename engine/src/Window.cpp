#include "Window.h"
#include <GL/eglew.h>
#include <iostream>

namespace Forge
{
    Window::Window(const std::string &title, int width, int height) : width(width), height(height)
    {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "[FORGE] SDL_Init Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Tell SDL we want OpenGL 3.3 Core profile
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // Create Window
        sdlWindow = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

        if (!sdlWindow)
        {
            std::cerr << "[FORGE] SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Create OpenGL context
        glContext = SDL_GL_CreateContext(sdlWindow);
        if (!glContext)
        {
            std::cerr << "[FORGE] SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(sdlWindow);
            sdlWindow = nullptr;
            return;
        }

        // Initialize GLEW
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "[FORGE] GLEW Error: " << glewGetErrorString(glewInit()) << std::endl;
            SDL_GL_DeleteContext(glContext);
            SDL_DestroyWindow(sdlWindow);
            sdlWindow = nullptr;
            SDL_Quit();
            return;
        }

        // V-Sync
        SDL_GL_SetSwapInterval(1);

        // Display OpenGL and GLEW versions
        std::cout << "[FORGE] OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "[FORGE] GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;

        open = true;
    }

    Window::~Window()
    {
        if (glContext)
        {
            SDL_GL_DeleteContext(glContext);
        }
        if (sdlWindow)
        {
            SDL_DestroyWindow(sdlWindow);
        }
        SDL_Quit();
    }

    // Move constructor
    Window::Window(Window &&other) noexcept
    {
        sdlWindow = other.sdlWindow;
        glContext = other.glContext;
        width = other.width;
        height = other.height;
        open = other.open;

        // Null out the other's resources
        other.sdlWindow = nullptr;
        other.glContext = nullptr;
        other.open = false;
    }

    // Move assignment operator
    Window &Window::operator=(Window &&other) noexcept
    {
        if (this != &other)
        {
            // Clean up current resources
            if (glContext)
            {
                SDL_GL_DeleteContext(glContext);
            }
            if (sdlWindow)
            {
                SDL_DestroyWindow(sdlWindow);
            }

            // Move resources from other
            sdlWindow = other.sdlWindow;
            glContext = other.glContext;
            width = other.width;
            height = other.height;
            open = other.open;

            // Null out the other's resources
            other.sdlWindow = nullptr;
            other.glContext = nullptr;
            other.open = false;
        }
        return *this;
    }

    bool Window::isOpen() const
    {
        return open;
    }

    void Window::swapBuffers()
    {
        if (sdlWindow)
        {
            SDL_GL_SwapWindow(sdlWindow);
        }
    }

    void Window::pollEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                open = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    open = false;
                }
            }
        }
    }
}