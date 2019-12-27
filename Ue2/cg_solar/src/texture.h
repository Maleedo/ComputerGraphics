//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#ifndef TEXTURE_H
#define TEXTURE_H
//=============================================================================

#include "gl.h"

//=============================================================================

/// class that handles texture io and GPU upload
class Texture
{
public:

    /// default constructor
    Texture();

    /// default destructor
    ~Texture();

    /// creates an empty texture on the GPU
    /// \param unit texure unit (important for use of multiple texturs in shader)
    /// \param type the type of the texture (1D, 2D, 3D etc.)
    /// \param minfilter interpolation filter for minification
    /// \param magfilter interpolation filter for magnification
    /// \param wrap texture coordinates wrap preference
    void init(GLenum unit, GLenum type, GLint minfilter, GLint magfilter, GLint wrap);

    /// uploads a texture to the GPU
    /// \param filename the location and name of the texture for upload
    bool loadPNG(const char* filename);


    /// creates a sunglow texture
    void createSunglowTexture();

    /// activates this texture for the predefined unit
    void bind();

    /// returns the texture id
    GLint id() const { return id_; }

private:

    /// texture ID on GPU
    GLuint id_;

    /// texture unit (important for use of multiple texturs in shader)
    GLenum unit_;

    /// the type of the texture (TEXTURE_1D,TEXTURE_2D, etc)
    GLint  type_;

    /// the minfilter setting
    GLint minfilter_;

};


//=============================================================================
#endif
//=============================================================================
