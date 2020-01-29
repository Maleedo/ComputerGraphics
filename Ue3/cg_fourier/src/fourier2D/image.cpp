//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#include "image.h"
#include "lodepng.h"
#include <iostream>
#include <cassert>

//=============================================================================


Signal2D::Signal2D(unsigned int _width, unsigned int _height)
{
    resize(_width, _height);
}


void Signal2D::resize(unsigned int _width, unsigned int _height)
{
    width_  = _width;
    height_ = _height;
    data_.clear();
    data_.resize(width_*height_, 0.0);
}


bool Signal2D::read(const char* _filename)
{
    // read block of RGBA values
    std::vector<unsigned char> rgba;
    unsigned error = lodepng::decode(rgba, width_, height_, _filename);
    if (error)
    {
        std::cout << "read error: " << lodepng_error_text(error) << std::endl;
        width_ = height_ = 0;
        return false;
    }

    // convert RGBA (char) to grey scale (float)
    data_.resize(width_*height_);
    for (unsigned int i=0; i<width_*height_; ++i)
    {
        data_[i] = (rgba[4*i] + rgba[4*i+1] + rgba[4*i+2]) / 3.0 / 255.0;
    }

    return true;
}


bool Signal2D::write(const char* _filename)
{
    // convert grey scale (float) to RGBA (char)
    std::vector<unsigned char> rgba(4*width_*height_);
    for (unsigned int i=0; i<width_*height_; ++i)
    {
        rgba[4*i] = rgba[4*i+1] = rgba[4*i+2] = (unsigned char)(255 * std::min(1.0, std::max(0.0, data_[i])));
        rgba[4*i+3] = 255;
    }

    // write RGBA to file
    unsigned error = lodepng::encode(_filename, rgba, width_, height_);
    if (error)
    {
        std::cout << "write error: " << lodepng_error_text(error) << std::endl;
        return false;
    }

    return true;
}


//-----------------------------------------------------------------------------


Spectrum2D::Spectrum2D(unsigned int _width, unsigned int _height)
{
    resize(_width, _height);
}


void Spectrum2D::resize(unsigned int _width, unsigned int _height)
{
    width_  = _width;
    height_ = _height;
    data_.clear();
    data_.resize(width_*height_, complex(0.0));
}


bool Spectrum2D::write(const char* _filename)
{
    // convert grey scale (float) to RGBA (char)
    std::vector<unsigned char> rgba(4*width_*height_);
    for (unsigned int i=0; i<width_*height_; ++i)
    {
        //float v = std::min(1.0, log(1.0f + abs(data_[i])));
        float v = abs(data_[i]);
        rgba[4*i] = rgba[4*i+1] = rgba[4*i+2] = (unsigned char)(255 * v);
        rgba[4*i+3] = 255;
    }

    // write RGBA to file
    unsigned error = lodepng::encode(_filename, rgba, width_, height_);
    if (error)
    {
        std::cout << "write error: " << lodepng_error_text(error) << std::endl;
        return false;
    }

    return true;
}


//=============================================================================
