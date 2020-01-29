//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <cassert>

//=============================================================================

// our complex number type
typedef std::complex<float>   complex;

// the signal is stored in a vector of float
typedef std::vector<float>    Signal;

// the frequency spectrum is stored in a vector of complex
typedef std::vector<complex>  Spectrum;


//=============================================================================


const complex e(float _x)
{
    return complex(cos(_x), sin(_x));
}


//-----------------------------------------------------------------------------

/**
 * Performs a discrete fourier transformation on a one-dimensional signal.
 * @param _f the signal the transformation is performed on.
 * @param _F the spectrum that results from the transformation.
 */
void dft(const Signal& _f, Spectrum& _F)
{
    _F.resize(_f.size());

    const int N = _f.size();
    int  k, i;
#pragma omp parallel for private(k,i)
    for (k=0; k<N; ++k)
    {
        _F[k] = 0.0;

        /**
         * @todo
         * Implement the discrete fourier transformation.
         */

    }
}


//-----------------------------------------------------------------------------

/**
 * Performs an inverse discrete fourier transformation on a one-dimensional signal.
 * @param _F the spectrum the transformation is performed on.
 * @param _f the signal that results from the transformation.
 */
void idft(const Spectrum& _F, Signal& _f)
{
    _f.resize(_F.size());

    const int N = _F.size();
    int  k, i;
#pragma omp parallel for private(k,i)
    for (i=0; i<N; ++i)
    {
        _f[i] = 0.0;

        /**
         * @todo
         * Implement the inverse discrete fourier transformation.
         * Note that `_f` stores real numbers. The real component of
         * a complex number x can be accessed with x.real().
         */

    }
}


//-----------------------------------------------------------------------------


int main(int argc, char** argv)
{
    /// Signal `f` in the spatial domain.
    Signal    f;

    /// The signal's frequency spectrum `F` in the frequency domain.
    Spectrum  F;



    // read input file
    {
        std::ifstream ifs(argv[1]);
        if (!ifs) { std::cerr << "read error\n"; exit(1); }
        float x;
        while (ifs && !ifs.eof())
        {
            ifs >> x;
            if (ifs && !ifs.eof()) f.push_back(x);
        }
        ifs.close();
    }
    const int N = f.size();


    // compute Fourier transform: f -> F
    dft(f, F);


    // plot frequency amplitudes
    {
        std::ofstream ofs("spectrum.dat");
        for (int k=0; k<N; ++k)
        {
            ofs << (k-N/2) << "  " << abs(F[k]) << std::endl;
        }
        ofs.close();
    }



    /**
     * @todo
     * Find the heart rate, which is the dominant frequency:
     * - Neglect DC component (frequency `0`).
     * - Neglect all negative frequencies
     */

    int   kmax = 0;
    float Fmax = 0.0;


    std::cout << "Dominant frequency: " << kmax << std::endl;

    /**
     * @todo
     * Filter the signal by removing, i.e. setting to zero, all frequencies above the dominant frequency.
     */



    // inverse Fourier transform: F -> f
    idft(F, f);


    // output filtered signal
    {
        std::ofstream ofs("filtered.dat");
        for (int i=0; i<N; ++i)
            ofs << f[i] << std::endl;
        ofs.close();
    }
}


//=============================================================================
