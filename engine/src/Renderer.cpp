#include "Renderer.h"
#include <iostream>

namespace Forge {


    // Minimal GLSL shaders written as strings for now
    // Later loaded from files

    static const std:: string VERTEX_SHADER = R"(
    #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec3 aColor;
        out vec3 fragColor;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            fragColor = aColor;
        }
    )";

    static const std:: string FRAGMENT_SHADER = R"(
        #version 330 core
        in vec3 fragColor;
        out vec4 outColor;
        void main() {
            outColor = vec4(fragColor, 1.0);
        }
    )";
    Renderer::Renderer() {
        setClearColor(0.117f, 0.117f, 0.117f, 1.0f);
        setupTestTriangle();
    }

    Renderer::~Renderer() {
        if (vertexBuffer) {
            glDeleteBuffers(1, &vertexBuffer);
        }
        if (vertexArray) {
            glDeleteVertexArrays(1, &vertexArray);
        }
    }

   

    void Renderer::setClearColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
    }
    

    void Renderer::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::setupTestTriangle() {
        // Simple triangle vertices with colors
        float vertices[] = {
            0.0f,  0.5f,  1.0f, 0.0f, 0.0f,  // top (red)
            0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // right (green)
            -0.5f, -0.5f,  0.0f, 0.0f, 1.0f   // left (blue)
        };
        
        // Create VAO and VBO
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        // Create and bind vertex buffer
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0); // Unbind VAO

        // Compile shaders and create shader program
        shader = std::make_unique<Shader>(VERTEX_SHADER, FRAGMENT_SHADER);

        if (!shader->isValid()) {
            std::cerr << "[Forge] Failed to create shader program." << std::endl;
        }

    }

    void Renderer::drawTestTriangle() {
        if (!shader || !shader->isValid()) {
            std::cerr << "[Forge] Cannot draw triangle: Shader program is not valid." << std::endl;
            return;
        }
        shader->bind();
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        shader->unbind();
    }
}