#pragma once

#include <glad/gl.h>

namespace our {

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D {
        // The OpenGL object name of this texture 
        GLuint name = 0;
    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name" 
        Texture2D() {
            //TODO: (Req 5) Complete this function
            // generate texture using name of texture
            glGenTextures(1, &this->name);
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D() {
            //TODO: (Req 5) Complete this function
            // delete texture using name of texture
            glDeleteTextures(1, &this->name);
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName() {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const {
            //TODO: (Req 5) Complete this function
            // bind texture to GL_TEXTURE_2D using name of texture
            glBindTexture(GL_TEXTURE_2D, this->name);
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind() {
            //TODO: (Req 5) Complete this function
            // unbind texture from GL_TEXTURE_2D by passing 0 to GL_TEXTURE_2D
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        Texture2D(const Texture2D &) = delete;

        Texture2D &operator=(const Texture2D &) = delete;
    };

}