#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"

#include <stb_image.h>

#include <iostream>

namespace Forge {
Texture::Texture(const std::string& filePath) {
    // Tell stb_image to flip images vertically
    // OpenGL expects bottom-left origin, most images are top-left
    // stbi_set_flip_vertically_on_load(true);

    int channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "[Forge] Failed to load texture: " << filePath << std::endl;
        std::cerr << "[Forge] stb_image: " << stbi_failure_reason() << std::endl;
        return;
    }

    // Generate and bind the texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Texture Wrapping - repeat the texture if coordinates go outside [0,1]
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering - use linear filtering for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // upload pixel data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the image memory from the cpu as the gpu has it now
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture

    std::cout << "[Forge] Loaded texture: " << filePath << " (" << width << "x" << height << ")"
              << std::endl;
}

// Deconstructor to clean up the texture resource
Texture::~Texture() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        std::cout << "[Forge] Deleted texture ID: " << textureID << std::endl;
    }
}
// Move constructor
Texture::Texture(Texture&& other) noexcept
    : textureID(other.textureID), width(other.width), height(other.height) {
    other.textureID = 0;  // Prevent the destructor from deleting the texture
    other.width = 0;
    other.height = 0;
}

// Move assignment operator
Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        // Clean up existing texture
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
        }

        // Transfer ownership of the texture
        textureID = other.textureID;
        width = other.width;
        height = other.height;

        // Reset the other texture to prevent double deletion
        other.textureID = 0;
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void Texture::bind(GLuint slot) const {
    //   if (slot > 31) {
    //     std::cerr << "[Forge] Texture slot " << slot
    //               << " is out of range. Max is 31." << std::endl;
    //     return;
    //   }
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
}  // namespace Forge