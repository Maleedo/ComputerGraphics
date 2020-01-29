//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#ifndef FOURIER_H
#define FOURIER_H

//=============================================================================

#include "image.h"

//=============================================================================

/**
 * Performs a discrete fourier transformation on a two-dimensional signal.
 * @param _f the signal the transformation is performed on.
 * @param _F the spectrum that results from the transformation.
 */
void dft(const Signal2D& _f, Spectrum2D& _F);

/**
 * Performs an inverse discrete fourier transformation on a two-dimensional signal.
 * @param _Fthe spectrum the transformation is performed on.
 * @param _f the signal that results from the transformation.
 */
void idft(const Spectrum2D& _F, Signal2D& _f);


//=============================================================================
#endif
//=============================================================================
