//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#ifndef IMAGE_H
#define IMAGE_H

//=============================================================================

#include <iostream>
#include <cassert>
#include <complex>
#include <vector>

//=============================================================================


typedef std::complex<double>   complex;


//=============================================================================


/// Simple class for 2D images
class Signal2D
{
public:


    /// constructor
    Signal2D(unsigned int _width=0, unsigned int _height=0);

    /// resize an image (destroys its content)
    void resize(unsigned int _width, unsigned int _height);

    /// read image from PNG file
    bool read(const char* _filename);

    /// write image to PNG file
    bool write(const char* _filename);

    /// get (read access)
    double operator()(unsigned int _x, unsigned int _y) const
    {
        assert(_x < width_);
        assert(_y < height_);
        return data_[_y*width_+_x];
    }


    /// get (read/write access)
    double& operator()(unsigned int _x, unsigned int _y)
    {
        assert(_x < width_);
        assert(_y < height_);
        return data_[_y*width_+_x];
    }

    /// get width of image
    unsigned int width()  const { return width_;  }
    /// get height of image
    unsigned int height() const { return height_; }


private:

    std::vector<double> data_;
    unsigned int width_, height_;
};



//=============================================================================


class Spectrum2D
{
public:

    /// constructor
    Spectrum2D(unsigned int _width=0, unsigned int _height=0);

    /// resize an image (destroys its content)
    void resize(unsigned int _width, unsigned int _height);

    /// write image to PNG file
    bool write(const char* _filename);

    /// get (read access)
    complex operator()(unsigned int _x, unsigned int _y) const
    {
        assert(_x < width_);
        assert(_y < height_);
        return data_[_y*width_+_x];
    }


    /// get (read/write access)
    complex& operator()(unsigned int _x, unsigned int _y)
    {
        assert(_x < width_);
        assert(_y < height_);
        return data_[_y*width_+_x];
    }

    /// get width of image
    unsigned int width()  const { return width_;  }
    /// get height of image
    unsigned int height() const { return height_; }


private:

    std::vector<complex> data_;
    unsigned int width_, height_;
};


//=============================================================================
#endif
//=============================================================================
