//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "texture.h"
#include <iostream>
#include <assert.h>
#include "lodepng.h"
#include <algorithm>

//=============================================================================


Texture::Texture() :
    id_(0)
{
}


//-----------------------------------------------------------------------------


Texture::~Texture()
{
    if (id_) glDeleteTextures(1, &id_);
}


//-----------------------------------------------------------------------------

bool Texture::loadPNG(const char* filename)
{
    if (!id_)
    {
        std::cerr << "Texture: initialize before loading!\n";
        return false;
    }
    std::cout << "Load texture " << filename << "\n" << std::flush;

    std::vector<unsigned char> img;
    unsigned width, height;

    unsigned error = lodepng::decode(img, width, height, filename);
    if (error)
    {
        std::cout << "read error: " << lodepng_error_text(error) << std::endl;
        return false;
	}

    // flip vertically in order to adhere to how OpenGL looks at images
    assert(height % 2 == 0);
    for (unsigned int x = 0; x < width/2; ++x) {
        assert(height % 2 == 0);
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int c = 0; c < 4; ++c) {
                unsigned char t = img[(x * height + y) * 4 + c];
                img[(x * height + y) * 4 + c] = img[((width - x - 1) * height + y) * 4 + c];
                img[((width - x - 1) * height + y) * 4 + c] = t;
            }
        }
	}


    // upload texture data
    glActiveTexture(unit_);
    glBindTexture(type_, id_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexImage2D(type_, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);


    if(minfilter_==GL_LINEAR_MIPMAP_LINEAR)
    {
        // comment out to disable mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return true;
}

//-----------------------------------------------------------------------------

void Texture::createSunglowTexture()
{
    std::vector<float> img;
    int width = 900;
    int height = 900;
    img.resize(width*height * 4);

    /** \todo Setup the texture for the sun billboard by filling `img` with color values.
    *	- Draw an opaque (alpha == 1) circle with a 150 pixel radius in its middle
    *	- Outside that circle the texture should become more and more transparent (alpha < 1) to mimic a nice glow effect
    *   - Make sure that your texture is fully transparent (alpha == 0) at its borders to avoid seeing visible edges
    *	- Experiment with the color and with how fast you change the transparency until the effect satisfies you
    **/

    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            img[(w*width + h) * 4 + 0] = 1.0;//red
            img[(w*width + h) * 4 + 1] = 1.0;//green
            img[(w*width + h) * 4 + 2] = 1.0;//blue
            img[(w*width + h) * 4 + 3] = 1.0;//alpha

        }
    }

    // upload texture data
    glActiveTexture(unit_);
    glBindTexture(type_, id_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexImage2D(type_, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &img[0]);


    if (minfilter_ == GL_LINEAR_MIPMAP_LINEAR)
    {
        // comment out to disable mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

//-----------------------------------------------------------------------------


void Texture::init(GLenum unit, GLenum type, GLint minfilter, GLint magfilter, GLint wrap)
{
    // remember this
    unit_ = unit;
    type_ = type;
    minfilter_ = minfilter;

    // activate texture unit
    glActiveTexture(unit_);

    // create texture object
    glGenTextures(1, &id_);
    glBindTexture(type_, id_);

    // set texture parameters
    glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, magfilter);
    glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, minfilter);
    glTexParameteri(type_, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(type_, GL_TEXTURE_WRAP_T, wrap);

}


//-----------------------------------------------------------------------------


void Texture::bind()
{
    assert(id_);
    glActiveTexture(unit_);
    glBindTexture(type_, id_);
}



//=============================================================================
