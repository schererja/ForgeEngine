#pragma once

#include <GL/glew.h>
#include <memory>
#include "Shader.h"

namespace Forge
{

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        // No Copying
        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        // // Moving is fine
        // Renderer(Renderer&& other) noexcept;
        // Renderer& operator=(Renderer&& other) noexcept;
        void setClearColor(float r, float g, float b, float a = 1.0f);
        void clear();
        void drawTestTriangle();

    private:
        GLuint vertexArray = 0;
        GLuint vertexBuffer = 0;
        std::unique_ptr<Shader> shader;
        void setupTestTriangle();
    };
}