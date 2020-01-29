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
#include <iostream>
#include <complex>

//=============================================================================


const complex e(float _x)
{
    return complex(cos(_x), sin(_x));
}


//-----------------------------------------------------------------------------

/**
 * @todo
 * Implement the discrete fourier transformation.
 * The naive version involves 4 for loops, which is not very efficient. To speed things up, try to separate
 * the stacked sums wherever it is possible. The goal is to divide the computation into two for loops with depth three
 * instead of one with depth 4.
 * You can parallelize your code with OpenMP.

 */


void dft(const Signal2D& _f, Spectrum2D& _F)
{
    const int M = _f.width();
    const int N = _f.height();
    int  k, l, i;

    _F.resize(M,N);
}



//-----------------------------------------------------------------------------

/**
 * @todo
 * Implement the inverse discrete fourier transformation.
 * Speed up your code analogous to the method in dft.
 */


void idft(const Spectrum2D& _F, Signal2D& _f)
{
    const int M = _F.width();
    const int N = _F.height();
    int  k, l, i, j;

    _f.resize(M,N);
}



//=============================================================================
