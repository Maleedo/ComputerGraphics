Solar System
============
Welcome to the second exercise for the lecture introduction to computer graphics. The focus of this exercise is OpenGL. Topics that we will cover are:
- hierarchical coordinate systems
- basic rendering in OpenGL
- stereoscopic viewing

Building under Linux and MacOS
------------------------------
Inside the exercise's top-level directory, execute the following commands:

    mkdir build
    cd build
    cmake ..
    make

The last command -- i.e. `make` -- compiles the application. Rerun it whenever you have added/changed code in order to recompile.

For MacOS, you will have to change the texture and model load paths from "../../texturename" to "../texturename" if you build without Xcode.

Dependencies
------------

When building on your own PC or Laptop under Linux, you might miss some dependencies. Those can be installed via:
    
    sudo apt-get install xorg-dev libglu1-mesa-dev libx11-dev freeglut3-dev libxmu-dev libxi-dev libgl-dev

Clear the build folder's content before running `cmake ..` again. 

Building under Mac using Xcode
------------------------------
If you prefer to use Xcode on Mac, to the following:

    mkdir build
    cd build
    cmake -G Xcode ..

and then open the generated Xcode project.

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
  * Use `Open Project` to load your `SolarSystem.sln`
  * On the right, there should be the solution explorer. Find the project `SolarViewer`, right click and choose `Set as StartUp Project`
  * Press CTRL + F5 to compile and run

Documentation
-------------
You find a prebuild documentation in the directory `doc`. Open the `doc/index.html` with your favorite webbrowser.

Textures and Copyright
----------------------
All earth textures are from the [NASA Earth Observatory](http://earthobservatory.nasa.gov/Features/BlueMarble/) and have been modified by Prof. Hartmut Schirmacher, Beuth Hochschule für Technik Berlin. The sun and saturn ring texture is from [Solarsystemscope.com](http://www.solarsystemscope.com/textures). All other textures are [Textures.com](from http://textures.forrest.cz/index.php?spgmGal=maps&spgmPic=14). The ship model if from [free3d.com](https://free3d.com).

Keyboard Settings
-----------------
  * arrow keys or left mouse key + drag: 	Navigation Camera
  * 9/0:									change camera's distance to the observed object
  * 1-8:									set camera to planets/sun
  * tab:									set camera to ship
  * W,S:									accelerate/decelerate Ship
  * A,D or left mouse key + drag:			rotate ship
  * r:										randomize planets' positions
  * g:										toggle greyscale
  * enter:									switch mono/stereo view mode
  * +/-:									increase/decrease time_step
  * space:									pause 
  * escape:									exit viewer
  

Recommended Order for the Exercises
-----------------------------------
* **Setup of an Animated Solar System (3 points):**
  * Start by placing all celestial bodies in space in `Solar_viewer::update_planets_positions()` (`src/solar_viewer.cpp`). The result should look similar to this:
    ![](planet_system_initial.png)
  * Each time the system is rendered, `Planet::angle_sun_` and `Planet::angle_self_` (`src/planet.h`) are increased in `Solar_viewer::timer()`. Use this to animate the solar system, i.e. make each body rotate around its own axis, around the sun, and -- in case of the moon -- around the earth. This will require complete the task in `Solar_viewer::update_planets_positions()` (`src/solar_viewer.cpp`).

?? solar_viewer_paint??

  *Allow a kind of navigation through the solar system and for a change of the camera's agle and eye's distance to the celestial body. Details can be found in `Solar_viewer::paint()` and `Solar_viewer::keyboard(...)` (`src/solar_viewer.cpp`).
A video for comparison can be found in the video directory (`videos/navigation.mp4`). If you look at the system from above and wait some timesteps, the result should look similar to this:
  ![](planet_system_rotating.png)

* **Phong Lighting (2 points):**
  Implement the basic phong shader model (`src/shader/phong.vert` and `src/shader/phong.frag`). The result should look similar to this:
  ![](venus_phong_shading.png)

* **Earth (2 points):**
  For the earth, replace the phong shader with the earth shader (`src/shader/earth.vert` and `src/shader/earth.frag`) that features a special night, gloss and clouds texture. The result can be seen in the video `videos/earth.mp4` and should look similar to this:
  ![](earth_final.png)

* **Spaceship and Sunglow (2 points):**
  * Rotating around planets is boring. Let's implement a spaceship. Complete the simple .off reader in `Ship::load_model(...)` to read the ship model. When this works, you can render the ship with your phong shader in `Solar_viewer::draw_scene(...)`. Make sure that you update the viewmatrix correctly, when you are in the ship (`Solar_viewer::paint()`).  The result can be seen in `videos/spaceship.mp4` and should look similar to this:
  ![](spaceship_moving.png)
  * Let the sun shine: implement a simple billboard which is a textured quad that always faces the viewer. Fill the texture with color values in `Texture::createSunglowTexture()` to mimic a fading aura around the sun. The result texture should look similar to this:
    ![](sunglow.png)
  * Then, render it in `Solar_viewer::draw_scene(...)` with the color shader. To make sure, that it always faces the viewer complete `Billboard::computeBillboardAngles(...)`. The result shoul look like
  ![](sun_billboard.png)
  * If you want a video comparison check out the `spaceship.mp4` in the video directory.

* **Stereo Rendering (1 points):**
  Add stereo rendering to `Solar_viewer::paint()` (`src/solar_viewer.cpp`).

* **Bonus Task (1 points):**
    * How about a more fancy planet? Like the beautiful Saturn with its asteroid rings? For this bonus task place both of them in our solar system.
    * The mesh of the rings has to be completed in `Saturn_Rings::initGlArrays()` (saturn_rings.h). Remember that the Saturn is slightly tilted, a comparison how it should look like can be found in the video directory (`videos/saturn.mp4`)
    * If you completed the sunglow task, you may notice that the rings and glow of the sun overlap in a weird way. Make sure that both of them are correctly rendered by choosing the right order.


* **Optional features:**
   - Extend your color shader with a time and angle-dependent vertex shift to get a flow-like effect for your sun surface (details in `Solar_viewer::draw_scene(...)` 8. and `color.vert`.
   - The navigation of your ship is easy but not very realistic. You could improve that by vectorial forces and velocitys to update your position. Moreover, the celestial bodies should have a gravitational effect on it.
   - Use the spaceship and billboards to create a little game: your ship might have a fuel tank, which empties over time and one has to collect fuel billboards to fill it or the user must fly to planets in a specific order (use the `Solar_viewer::randomize_planets()` (key: r) function to get more challenging planet positions faster).
   - Maybe you noticed, that we just draw 6 of our 8 (poor pluto) solar systems' planets. But you can find all textures (plus pluto) in you `textures` folder. So feel free to render them all.
   - In reality, a planet's orbit is not a circle but an ellipse. You can combine scaling and rotations to achieve this effect.
   - We are always interested in your own ideas!

