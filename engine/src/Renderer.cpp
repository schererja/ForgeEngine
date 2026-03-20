#include "Renderer.h"

namespace Forge {

    Renderer::Renderer() {
        setClearColor(clearR, clearG, clearB, clearA);
    }

    Renderer::Renderer(Renderer&& other) noexcept {
        clearR = other.clearR;
        clearG = other.clearG;
        clearB = other.clearB;
        clearA = other.clearA;
    }

    Renderer& Renderer::operator=(Renderer&& other) noexcept {
        if (this != &other) {
            clearR = other.clearR;
            clearG = other.clearG;
            clearB = other.clearB;
            clearA = other.clearA;
        }
        return *this;
    }

    void Renderer::setClearColor(float r, float g, float b, float a) {
        clearR = r;
        clearG = g;
        clearB = b;
        clearA = a;
        glClearColor(clearR, clearG, clearB, clearA);
    }

    void Renderer::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}