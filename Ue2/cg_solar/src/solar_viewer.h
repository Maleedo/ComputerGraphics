//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#ifndef SOLAR_VIEWER_H
#define SOLAR_VIEWER_H
//=============================================================================


#include "gl.h"
#include "glfw_window.h"
#include "glmath.h"

#include "shader.h"
#include "texture.h"
#include "planet.h"
#include "ship.h"
#include "billboard.h"
#include "saturn_rings.h"


//=============================================================================


enum PLANET {SUN=0, MERCURY=1,VENUS=2, EARTH=3, MOON=4, MARS=5};


//=============================================================================


/// OpenGL viewer that handles all the rendering for us
class Solar_viewer : public GLFW_window
{
public:

    /// default constructor
    /// \_title the window's title
    /// \_width the window's width
    /// \_height the window's height
    Solar_viewer(const char* _title, int _width, int _height);


protected:

    /// function that is called on the creation of the widget for the initialisation of OpenGL
    virtual void initialize();

    /// resize function - called when the window size is changed
    virtual void resize(int width, int height);

    /// paint function - called when the window should be refreshed
    virtual void paint();

    /// updates all positions and model matrices of planets
    virtual void update_planets_positions();

    /// keyboard interaction
    virtual void keyboard(int key, int scancode, int action, int mods);

    /// mouse interaction
    virtual void mouse(int button, int action, int mods);

    /// mouse motion
    virtual void motion(double xpos, double ypos);

    /// mouse wheel
    virtual void scroll(double xoffset, double yoffset);


    /// function that draws the planet system
    /// \param _projection the projection matrix for the scene
    /// \param _view the view matrix for the scene
    void draw_scene(mat4& _projection, mat4& _view);

    /// update function on every timer event (controls the animation)
    virtual void timer();

    void randomize_planets();


private:

    /// the sun object
    Planet sun_;
    /// the mercury object
    Planet mercury_;
    /// the venus object
    Planet venus_;
    /// the earth object
    Earth  earth_;
    /// the moon object
    Planet moon_;
    /// the mars object
    Planet mars_;
    /// the jupiter object
    Planet jupiter_;
    /// the saturn object
    Planet saturn_;
    Saturn_Rings saturn_rings_;
    /// the stars object
    Planet stars_;
    /// spaceship object
	Ship ship_;
    /// sunglow billboard
	Billboard sunglow_;

    std::vector<Planet*> planets_;


    /// default color shader (renders only the texture)
    Shader   color_shader_;
    /// phong shader (renders texture and basic illumination)
    Shader	 phong_shader_;
    /// earth shader (renders the earth using multi texturing)
	Shader   earth_shader_;
	/// earth shader (renders the earth using multi texturing)
	Shader   ship_shader_;

    /// interval for the animation timer
    bool  timer_active_;
    /// update factor for the animation
    float time_step_;

    /// state whether the rendering should be in color or not
    bool greyscale_;

    /// state whether the rendering should be in stereo or not
    enum { MONO, STEREO_ANAGLYPH, STEREO_SPLIT } view_mode_;

    /// the field of view for the camera
    float fovy_;
    /// the near plane for the virtual camera
    float near_;
    /// the far plane for the virtual camera
    float far_;

    /// which planet are we looking at (control with key 1-6)
    Planet* look_at_;

    /// rotation in x direction around the planet/sun from the original point
    float x_angle_;
    /// rotation in y direction around the planet/sun from the original point
    float y_angle_;
    /// eye's distance in radii from the observed planet
    float dist_factor_;
    /// true, if we look at the spaceship
    bool in_ship_;

    /// current viewport dimension
    int  width_, height_;

    /// true if left mouse button is pressed
    bool mouse_down_;

    /// true if mouse is inside window while dragging
    bool mouse_inside_;
    float last_x_, last_y_;
    float yUp_;
};


//=============================================================================
#endif
//=============================================================================
