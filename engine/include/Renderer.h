#pragma once

#include <GL/glew.h>

namespace Forge {
   
    class Renderer {
        public:
            Renderer();
            ~Renderer() = default;

            // No Copying
            Renderer(const Renderer&) = delete;
            Renderer& operator=(const Renderer&) = delete;
            
            // Moving is fine
            Renderer(Renderer&& other) noexcept;
            Renderer& operator=(Renderer&& other) noexcept;
            void setClearColor(float r, float g, float b, float a = 1.0f);
            void clear();
        private:
            float clearR = 0.117f;
            float clearG = 0.117f;
            float clearB = 0.117f;
            float clearA = 1.0f;
        };
}