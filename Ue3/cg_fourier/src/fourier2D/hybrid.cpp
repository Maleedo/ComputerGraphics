//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#include "fourier.h"
#include "image.h"
#include <iostream>

//=============================================================================


int main(int argc, char** argv)
{
    Signal2D    f1, f2, f;
    Spectrum2D  F1, F2, F;


    // read both images
    f1.read(argv[1]);
    f2.read(argv[2]);
    assert(f1.width() == f2.width());
    assert(f1.height() == f2.height());
    const int M = f1.width();
    const int N = f1.height();
    F.resize(M,N);


    // compute DFT
    dft(f1, F1);
    dft(f2, F2);


    /**
     * @todo
     * Create a hybrid by following these steps:
     * - Apply a low pass filter (Gauss) to the first spectrum.
     * - Apply a high pass filter to the second spectrum.
     *   This means subtracting its low pass filtered version from itself.
     * - Combine both spectra by calculating their sum.
     */


    // compute inverse DFT
    idft(F, f);


    // write image
    f.write("hybrid.png");
}


//=============================================================================
