#pragma once

#include <GL/glew.h>
#include <string>

namespace Forge {
    class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        // Rule of Five - delete copy constructor and copy assignment operator - no copying
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        // Moving is fine
        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        void bind() const;
        void unbind() const;

        bool isValid() const { return programID != 0; }

    private:
        GLuint programID = 0;

        GLuint compileShader(GLenum type, const std::string& source);


    };
}