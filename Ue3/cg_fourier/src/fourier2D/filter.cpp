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
#include "fourier.h"
#include <iostream>

//=============================================================================


int main(int argc, char** argv)
{
    Signal2D    f;
    Spectrum2D  F;


    // read image
    std::cout << "read image" << std::flush;
    f.read(argv[1]);
    f.write("0-image.png");
    const int M = f.width();
    const int N = f.height();
    std::cout << ", done: " << M << " x " << N << std::endl;


    // compute DFT
    std::cout << "compute DFT" << std::flush;
    dft(f, F);
    F.write("1-spectrum.png");
    std::cout << ", done\n";


    /**
     * @todo
     * Apply a low pass filter to the spectrum.
     */

    std::cout << "filter image" << std::flush;


    F.write("2-filtered-spectrum.png");
    std::cout << ", done\n";


    // compute inverse DFT
    std::cout << "compute iDFT" << std::flush;
    idft(F, f);
    std::cout << ", done\n";


    // write image
    std::cout << "write image" << std::flush;
    f.write("3-filtered-image.png");
    std::cout << ", done\n";
}


//=============================================================================
