Fun with Fourier
================

Building under Linux
--------------------
Inside the exercise's top-level directory, execute the following commands:

    mkdir build
    cd build
    cmake ..
    make

The last command -- i.e. `make` -- compiles the application. Rerun it whenever you have added/changed code in order to recompile.

Documentation
-------------
You find a prebuild documentation in the directory `documentation`. Open the `documentation/index.html` with your favorite webbrowser.

Building under MacOS
--------------------
Inside the project's directory, execute the following commands:

    mkdir xcode
    cd xcode
    cmake -G Xcode ..
    open Fourier.xcodeproj

Building under Windows Visual Studio
------------------------------------
  * Install Visual Studio Community 2013 or later
  * You will be asked, if you want to install additional packages. Make sure that you check the c++ development option.
  * Inside the exercise's top-level directory create a new `build` folder (CTRL + SHIFT + N)
  * Install [CMake] (https://cmake.org/download/)
  * Start the cmake-gui.exe
  * Click `Browse Source` and select the exercise's top-level directory
  * Click `Browse Build` and select the created `build` folder
  * Click `Configure` and select your Visual Studio version
  * Start Configuring. 
  * If no major errors occur, click `Generate`
  * Start Visual Studio
  * Use `Open Project` to load your Visual Studio solution file (.sln) in the build directory
  * On the right, there should be the solution explorer. Find the fourier1D project, right click and choose `Set as StartUp Project`
  * You can specify the command arguments for the different exercises via project -> properties -> debugging -> command arguments
  * Press CTRL + F5 to compile and run

Running the one-dimensional Fourier transformation
--------------------------------------------------
Inside the `build` directory, execute the following command to run the one-dimensional Fourier Transformation:

    ./fourier1D ../data/signal-50Hz.dat

This creates two files: `filtered.dat` and `spectrum.dat`. To visualize the data, you may use a program like *gnuplot*, *matlab*, *octave* or *matplotlib*.

With *gnuplot*, the following input (still inside `build`) is sufficient to create labeled plots:

    plot 'spectrum.dat' with lines

or

    plot '../data/signal-50Hz.dat' with lines, 'filtered.dat' with lines

Type `exit` to exit *gnuplot*.

Our results, for you to compare yours to, are provided in `data/results/filter1D`.

Running the two-dimensional Fourier transformation
--------------------------------------------------
Inside the `build` directory, execute the following command to run the two-dimensional Fourier Transformation:

    ./filter ../data/miezen.png

This creates four files: `0-image.png`, `1-spectrum.png`, `2-filtered-spectrum.png` and `3-filtered-image.png`. Open them with any appropriate image viewer.

Results are provided in `data/results/filter2D`.

Running the hybrid image generator
----------------------------------
Inside the `build` directory, execute the following command to create hybrid images:

    ./hybrid ../data/f.png ../data/m.png

This creates the file `hybrid.png`. Open it with any appropriate image viewer.

Results are provided in `data/results/hybrid`.

Suggested order for the exercises
---------------------------------
* **One-Dimensional Fourier Transformations (3 Points):**
  Start by getting the the one-dimensional Fourier transformation to work. In `src/fourier1D/fourier1D.cpp`, finish the methods `dft` and `idft`. Note the use of complex numbers, which are part of the C++ standard library. Afterwards, determine the heart rate in `data/signal-50Hz.dat` by filtering the signal in the `main` method.

* **Two-Dimensional Fourier Transformations (3 Points):**
  (1 point if it works, 3 if its fast as well)
  Once you are comfortable with one-dimensional Fourier transformations, move on to the two-dimensional case. Implement the methods `dft` and `idft` in `src/fourier2D/fourier.cpp`. In order to avoid complexity `O(N^4)`, separate the stacked sums where possible. Use OpenMP for parallelization.

* **Filtering (1 Points):**
  Todo: Add small picture.
  Equipped with these two methods, edit the `main` method in `src/fourier2D/filter.cpp` and add a low pass filter. Try alternative filters as well and observe how the spectrum changes.

* **Hybrid Images (1 Points):**
  Lastly, finish `src/fourier2D/hybrid.cpp` to be able to combine two images into one hybrid image. Read up on the process in the paper mentioned in the lecture and the lecture's slides.

