// @author Abdelaziz Salah

#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#include <iostream>

/*
    this is a util function used to generate an empty texture
    @param format: the format of the texture
    @param size: the size of the texture
*/
our::Texture2D *our::texture_utils::empty(GLenum format, glm::ivec2 size)
{
    // our::Texture2D *texture = new our::Texture2D();
    // TODO: (Req 11) Finish this function to create an empty texture with the given size and format

    our::Texture2D *texture = new our::Texture2D();

    // Bind the texture object
    // binding means that all the following commands will affect this texture object
    texture->bind();

    // Set the texture data to NULL to create an empty texture
    // I wanted to use this function, but when I did a search I found that the glTexStorage2D function is better, and I wanted to use another function
    // to know more about it, I found that it is better to use it because it is faster and more efficient.
    // glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, nullptr);

    glTexStorage2D(GL_TEXTURE_2D, 1, format, size.x, size.y);

    // Set some default texture filtering parameters
    // when using TexStorage2D, there is no need to use those functions.
    // texture->set_min_filter(our::TextureMinFilter::Linear);
    // texture->set_mag_filter(our::TextureMagFilter::Linear);

    // Unbind the texture object
    texture->unbind();

    // Return the texture object
    return texture;
}

our::Texture2D *our::texture_utils::loadImage(const std::string &filename, bool generate_mipmap)
{
    glm::ivec2 size;
    int channels;
    // Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    // We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    // Load image data and retrieve width, height and number of channels in the image
    // The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    // Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char *pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if (pixels == nullptr)
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D *texture = new our::Texture2D();
    // Bind the texture such that we upload the image data to its storage
    // TODO: (Req 5) Finish this function to fill the texture with the data found in "pixels"

    // Bind the texture object
    texture->bind();
    // create a new two-dimensional texture object in memory and initialize it with data.
    // Type of texture: GL_TEXTURE_2D
    // Level of detail: 0
    // Internal format: GL_RGBA8
    // Width: size.x
    // Height: size.y
    // Border: 0
    // Format: GL_RGBA
    // Type: GL_UNSIGNED_BYTE
    // Data: pixels (the image data)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // if generate_mipmap variable is true, generate mipmaps for the texture
    // Mipmaps are pre-calculated chains of optimized textures. Each texture in the chain is a progressively lower resolution representation of the same image.
    if (generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    // The purpose of this function is to free the memory that was allocated by the stbi_load function, which is used to load an image into memory.
    stbi_image_free(pixels); // Free image data after uploading to GPU
    return texture;
}