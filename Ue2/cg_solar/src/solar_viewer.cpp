//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "solar_viewer.h"
#include "glmath.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

//=============================================================================

// start version scales
#define D_RADIUS_SCALE_SMALL 15.0f
#define D_RADIUS_SCALE_BIG 6.0f
#define D_DISTANCE_SCALE_SMALL 0.03f
#define D_DISTANCE_SCALE_BIG 0.015f
#define D_DISTANCE_SCALE_MOON 1.5f

// better version <-- use this as soon as you finished the first 2 tasks
//#define D_RADIUS_SCALE_SMALL 12.0f
//#define D_RADIUS_SCALE_BIG 7.0f
//#define D_DISTANCE_SCALE_SMALL 0.07f
//#define D_DISTANCE_SCALE_BIG 0.07f
//#define D_DISTANCE_SCALE_MOON 1.5f

// realistic version <-- use this just if you are interested in realistic scales
// or if you want to get a 'lost in space' feeling (use the spaceship and try to reach some of the other planets)
//#define D_RADIUS_SCALE_SMALL 1.0f
//#define D_RADIUS_SCALE_BIG 1.0f
//#define D_DISTANCE_SCALE_SMALL 1.0f
//#define D_DISTANCE_SCALE_BIG 1.0f
//#define D_DISTANCE_SCALE_MOON 1.0f

Solar_viewer::Solar_viewer(const char* _title, int _width, int _height)
    : GLFW_window(_title, _width, _height),
      saturn_rings_(1.3,2.0,150),
      sun_    ("sun", 0.0f,              2.0f*(float)(M_PI)/26.0f,   1.0f,    0.0f),
      sunglow_(sun_.radius_*3.0),
      mercury_("mercury", 2.0f*(float)(M_PI)/116.0f,  2.0f*(float)(M_PI)/58.5f,   D_RADIUS_SCALE_SMALL*0.0035f, D_DISTANCE_SCALE_SMALL*-50.5f),
      venus_  ("venus", 2.0f*(float)(M_PI)/225.0f,  2.0f*(float)(M_PI)/243.0f,  D_RADIUS_SCALE_SMALL*0.0087f,   D_DISTANCE_SCALE_SMALL*-78.4f),
      earth_  ("earth", 2.0f*(float)(M_PI)/365.0f,  2.0f*(float)(M_PI),        D_RADIUS_SCALE_SMALL*0.0091f,   D_DISTANCE_SCALE_SMALL*-109.6f),
      moon_   ("moon", 2.0f*(float)(M_PI)/27.0f,   0.0f,  D_RADIUS_SCALE_SMALL*0.0024f,   D_DISTANCE_SCALE_MOON*0.26f),
      mars_   ("mars", 2.0f*(float)(M_PI)/687.0f,  2.0f*(float)(M_PI)/1.88, D_RADIUS_SCALE_SMALL*0.0048f,D_DISTANCE_SCALE_SMALL*-179.4f),
      jupiter_("jupiter", 2.0f*(float)(M_PI)/4335.0f,  2.0f*(float)(M_PI)/0.41f, D_RADIUS_SCALE_BIG*0.1027f,D_DISTANCE_SCALE_BIG*-586.6f),
      saturn_ ("saturn", 2.0f*(float)(M_PI)/10715.0f,  2.0f*(float)(M_PI)/0.44f, D_RADIUS_SCALE_BIG*0.0866f,D_DISTANCE_SCALE_BIG*-1079.7f),
      stars_  ("stars", 0.0f, 0.0f, D_DISTANCE_SCALE_BIG*2500.0f, 0.0f)
{
    planets_.resize(7);
    planets_[0] = &mercury_; planets_[1] = &venus_; planets_[2] = &earth_; planets_[3] = &mars_; planets_[4] = &jupiter_; planets_[5] = &saturn_; planets_[6] = &moon_;
    // start animation
    timer_active_ = true;
    time_step_ = 1.0f/24.0f; // one hour

    // rendering parameters
    greyscale_     = false;
    view_mode_ = MONO;
    fovy_ = 45;
    near_ = 0.01f;
    far_  = 20;

    // viewing angle
    look_at_ = &earth_;
    x_angle_ = 0.0f;
    y_angle_ = 0.0f;
    dist_factor_ = 4.5f;

    ship_.pos_ = look_at_->pos_ - vec4(0.0f, 0.0f, dist_factor_*look_at_->radius_, 0.0f);
    ship_.direction_ = vec4(0.0f, 0.0f, 1.0f,0.0f);
    //ship_.radius_ = 0.03*earth_.radius_;
    in_ship_ = false;

    mouse_down_ = false;
    last_x_ = -1;
    last_y_ = -1;

    srand((unsigned int)time(NULL));
}

//-----------------------------------------------------------------------------


void Solar_viewer::initialize()
{
    // set initial state
    glClearColor(1,1,1,0);
    glEnable(GL_DEPTH_TEST);


    // setup sun texture
    sun_.tex_.init(GL_TEXTURE0,GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR, GL_REPEAT);

    // setup planet's texure
    for(Planet* planet : planets_)
    {
        planet->tex_.init(GL_TEXTURE0,GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR, GL_REPEAT);
    }

    // setup earth special textures
    earth_.night_.init(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    earth_.cloud_.init(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    earth_.gloss_.init(GL_TEXTURE3, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

    moon_.tex_.init(GL_TEXTURE0,GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR, GL_REPEAT);
    mars_.tex_.init(GL_TEXTURE0,GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR, GL_REPEAT);

    // setup remaining textures
    stars_.tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    ship_.tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    sunglow_.tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    saturn_rings_.tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

    sunglow_.tex_.createSunglowTexture();


    // load textures
#if defined(_WIN32) || defined(__APPLE__)
    sun_.tex_.loadPNG("../../textures/sun.png");
    mercury_.tex_.loadPNG("../../textures/mercury.png");
    venus_.tex_.loadPNG("../../textures/venus.png");

    earth_.tex_.loadPNG(  "../../textures/day.png");
    earth_.night_.loadPNG("../../textures/night.png");
    earth_.cloud_.loadPNG("../../textures/clouds.png");
    earth_.gloss_.loadPNG("../../textures/gloss.png");

    moon_.tex_.loadPNG("../../textures/moon.png");
    mars_.tex_.loadPNG("../../textures/mars.png");
    stars_.tex_.loadPNG("../../textures/stars2.png");

    jupiter_.tex_.loadPNG("../../textures/jupiter.png");
    saturn_.tex_.loadPNG("../../textures/saturn.png");
    saturn_rings_.tex_.loadPNG("../../textures/saturn_rings.png");

    ship_.load_model("../../meshes/spaceship.off");
    ship_.tex_.loadPNG("../../textures/ship.png");

#else
    sun_.tex_.loadPNG("../textures/sun.png");
    mercury_.tex_.loadPNG("../textures/mercury.png");
    venus_.tex_.loadPNG("../textures/venus.png");

    earth_.tex_.loadPNG(  "../textures/day.png");
    earth_.night_.loadPNG("../textures/night.png");
    earth_.cloud_.loadPNG("../textures/clouds.png");
    earth_.gloss_.loadPNG("../textures/gloss.png");

    moon_.tex_.loadPNG("../textures/moon.png");
    mars_.tex_.loadPNG("../textures/mars.png");
    stars_.tex_.loadPNG("../textures/stars2.png");

    jupiter_.tex_.loadPNG("../textures/jupiter.png");
    saturn_.tex_.loadPNG("../textures/saturn.png");
    saturn_rings_.tex_.loadPNG("../textures/saturn_rings.png");

    ship_.tex_.loadPNG("../textures/ship.png");
    ship_.load_model("../meshes/spaceship.off");
    ship_.initGlArrays();
#endif

    // setup shader
#if defined(_WIN32) || defined(__APPLE__)
    color_shader_.load("../../src/shader/color.vert", "../../src/shader/color.frag");
    phong_shader_.load("../../src/shader/phong.vert", "../../src/shader/phong.frag");
        earth_shader_.load("../../src/shader/earth.vert", "../../src/shader/earth.frag");
#else
    color_shader_.load("../src/shader/color.vert", "../src/shader/color.frag");
    phong_shader_.load("../src/shader/phong.vert", "../src/shader/phong.frag");
    earth_shader_.load("../src/shader/earth.vert", "../src/shader/earth.frag");
#endif

}


//-----------------------------------------------------------------------------

void Solar_viewer::keyboard(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {

        case GLFW_KEY_1:
        {
            in_ship_ = false;
            look_at_ = &sun_;
            break;
        }

        case GLFW_KEY_2:
        {
            in_ship_ = false;
            look_at_ = &mercury_;
            break;
        }
        case GLFW_KEY_3:
        {
            in_ship_ = false;
            look_at_ = &earth_;
            break;
        }
        case GLFW_KEY_4:
        {
            in_ship_ = false;
            look_at_ = &moon_;
            break;
        }
        case GLFW_KEY_5:
        {
            in_ship_ = false;
            look_at_ = &mars_;
            break;
        }
        case GLFW_KEY_6:
        {
            in_ship_ = false;
            look_at_ = &jupiter_;
            break;
        }
        case GLFW_KEY_7:
        {
            in_ship_ = false;
            look_at_ = &saturn_;
            break;
        }
        case GLFW_KEY_8:
        {
            in_ship_ = false;
            look_at_ = planets_[key - 50];
            break;
        }

        case GLFW_KEY_TAB:
        {
            in_ship_ = !in_ship_;
            if(in_ship_)

                time_step_ = 0.002f;
            break;
        }

        /** \todo Implement the ability to change the viewer's distance to the celestial body.
         *    - key 9 should increase and key 0 should decrease the `dist_factor_`
         *    - make sure that `2.5 < dist_factor_ < 20.0`
         * Note: the mouse wheel also activates zooming, if you finish this task.
         */

        case GLFW_KEY_0:
        {
            if(2.5 < dist_factor_)
            {
                dist_factor_ -= 0.50;
            }
            break;
        }

        case GLFW_KEY_9:
        {
            if(dist_factor_ < 20.0 )
            {
                dist_factor_ += 0.50;
            }
            break;
        }

        case GLFW_KEY_R:
        {
            randomize_planets();
            break;
        }

        case GLFW_KEY_G:
        {
            greyscale_ = !greyscale_;
            break;
        }

        case GLFW_KEY_S:
        case GLFW_KEY_W:
        {
            if (in_ship_)
            {
                float accel = (key == GLFW_KEY_W) ? 0.0002 : -0.0002;
                ship_.accelerate(accel);
            }
            break;
        }

        case GLFW_KEY_D:
        case GLFW_KEY_A:
        {
            if (in_ship_)
            {
                float accel = (key == GLFW_KEY_A) ? 0.02 : -0.02;
                ship_.accelerate_angular(accel);
            }
            break;
        }

        case GLFW_KEY_ENTER:
        {
            switch (view_mode_)
            {
                case MONO:
                    view_mode_ = STEREO_ANAGLYPH;
                    break;
                case STEREO_ANAGLYPH:
                    view_mode_ = STEREO_SPLIT;
                    break;
                case STEREO_SPLIT:
                    view_mode_ = MONO;
                    break;
            }
            break;
        }

        case GLFW_KEY_LEFT:
        {
            y_angle_ += 10.0*yUp_;
            break;
        }

        case GLFW_KEY_RIGHT:
        {
            y_angle_ -= 10.0*yUp_;
            break;
        }

        case GLFW_KEY_DOWN:
        {
            x_angle_ -= 10.0;
            break;
        }

        case GLFW_KEY_UP:
        {
            x_angle_ += 10.0;
            break;
        }

        case GLFW_KEY_SPACE:
        {
            timer_active_ = !timer_active_;
            break;
        }

        case GLFW_KEY_P:
        case GLFW_KEY_KP_ADD:
        case GLFW_KEY_RIGHT_BRACKET:
        case GLFW_KEY_EQUAL:
        {
            if(in_ship_) break;

            time_step_ *= 2.0f;
            time_step_ = std::min(time_step_, 80.0f);
            std::cout << "Time step: " << time_step_ << " days" << std::endl;
            break;
        }

        case GLFW_KEY_M:
        case GLFW_KEY_KP_SUBTRACT:
        case GLFW_KEY_SLASH:
        case GLFW_KEY_MINUS:
        {
            if(in_ship_) break;

            time_step_ *= 0.5f;
            time_step_ = std::max(time_step_, 0.01f);
            std::cout << "Time step: " << time_step_ << " days" << std::endl;
            break;
        }

        case GLFW_KEY_ESCAPE:
        {
            glfwSetWindowShouldClose(window_, GL_TRUE);
            break;
        }

        }
    }
}


//-----------------------------------------------------------------------------

void
Solar_viewer::mouse(int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        mouse_down_ = true;
        mouse_inside_ = true;
    }


    if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
    {
        mouse_down_ = false;
        last_x_ = -1;
        last_y_ = -1;
    }
}

void Solar_viewer::motion(double xpos, double ypos)
{
    if(mouse_down_)
    {
        if(last_x_ < 0 || last_y_ < 0)
        {
            last_x_ = xpos;
            last_y_ = ypos;
            return;
        }

        if(!in_ship_)
        {
            x_angle_ -= (ypos - last_y_)/(double)height_*200.0;
            y_angle_ -= (xpos - last_x_)/(double)width_*200.0*yUp_;
        }
        else
        {
            ship_.angle_ -= (xpos - last_x_)/(double)width_*200.0;
        }

        last_x_ = xpos;
        last_y_ = ypos;
    }
}

void Solar_viewer::scroll(double xoffset, double yoffset)
{
    (yoffset > 0) ? keyboard(GLFW_KEY_9, 0, GLFW_PRESS, 0) : keyboard(GLFW_KEY_0, 0, GLFW_PRESS, 0);
}

//-----------------------------------------------------------------------------


void Solar_viewer::timer()
{
    if (timer_active_)
    {
        sun_.time_step(time_step_);
        for(Planet* planet : planets_)
        {
            planet->time_step(time_step_);
        }
        ship_.update_ship();
        update_planets_positions();
    }
}


//-----------------------------------------------------------------------------


void Solar_viewer::resize(int _width, int _height)
{
    width_  = _width;
    height_ = _height;
    glViewport(0, 0, _width, _height);
}


//-----------------------------------------------------------------------------


void Solar_viewer::paint()
{
    // clear framebuffer and depth buffer first
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    /** \todo Implement a kind of navigation through the solar system.
     *   - Allow rotation of the camera, by modifying the view matrix.
     *     `x_angle_` and `y_angle` hold the neccesary information and are updated when certain keys are pressed (see `Solar_viewer::keyboard(...)`).
     *     `dist_factor_` is the number of radii from the center of the planet to the camera.
  ?? *   - If `x_angle_` and `y_angle_`are zero, the camera should be translated in positive z-direction from the planet's center.
  ?? *   - The camera should always face the planet's center. The rotation is meant to be around the planet'center not around the camera's center.
     *   - If one of the keys `1` to `8` is pressed, the camera should face the respective celestial body (compare `Solar_viewer::keyboard(...)`).
     *     `look_at_` is updated in this case and stores a pointer to the current looked at planet.
     *   - To support changing distance to the body see `Solar_viewer::keyboard(...)`
  ?? *   - If you are in the spaceship (member in_ship_), the camera should hover slightly behind it and rotate along with it.
     * Hint: To understand what effect is desired, it helps to watch the solution videos.
     */
    mat4  projection;
    mat4  view;
    float radius;
    vec4  eye;
    vec4  center;
    vec4  up;


    eye    = vec4(0,0,7,1.0);
    radius = sun_.radius_;
    center = sun_.pos_;
    up     = vec4(0,1,0,0);
    view   = mat4::look_at(look_at_->pos_ + vec3(0,0,dist_factor_), look_at_->pos_, up) * mat4::rotate_x(x_angle_) * mat4::rotate_y(y_angle_);

    yUp_ = up[1];

    vec3 billboard_to_eye = normalize(vec3(eye) - vec3(sun_.pos_));
    sunglow_.computeBillboardAngles(billboard_to_eye);

    // mono rendering
    if (view_mode_ == MONO)
    {
        projection = mat4::perspective(fovy_, (float)width_/(float)height_, near_, far_);
        draw_scene(projection, view);
    }


    // stereo rendering
    else
    {
        /** \todo Implement stereo rendering.
         *    1. Calculate parameters for the left and right eye.
         *    2. Use `mat4::frustum(...)` to construct your two projection matrices.
         *    3. Translate the `view` matrix to get one for the left and one for the right eye.
         *    3. Render two images, one for the left and one for the right eye using `draw_scene(...)`;.
         *      - For `view_mode_ == STEREO_ANAGLYPH`, look up `glColorMask` and use it to enable filtering of red/cyan.
         *		  Setting the rendering to greyscale (by pressing `g`) will show the effect best.
         *		- In view_mode_ == STEREO_SPLIT, you can use `glViewport` to render the left-eye image on the left (or upper)
         *		 screen half and the right-eye image on the right (or lower) screen half.
         *      - Do not forget to reset viewport and color mask.
		 *
         *		Hint: You need to clear the depth buffer between the two `draw_scene` calls.
         *	 
         */

        double focal_distance = distance(eye, center) + 3.0*radius;
        double eye_separation = focal_distance * 0.008;

    }
}

//-----------------------------------------------------------------------------

void Solar_viewer::update_planets_positions()
{
    sun_.model_matrix_ = mat4::rotate_y(sun_.angle_self_) * mat4::scale(sun_.radius_);

    /** \todo Update `model_matrix_` and position (`pos_`) for each planet/moon.
    * 1. Start by positioning all celestial bodies (mercury, venus, earth, mars, jupiter and the earth's moon) considered  along one axis.
    *   Hints:
    *   - The distance from the rotation's center and the scale to be applied is given for each instance of `Planet`
    *   (that class is used for all bodies, including the moon, even though the moon is no planet).
??  *   - Consider disabling face culling in order to render backfaces.
    *   - See glmath.h/cpp for an overview about implemented matrices.
    *   - `Planet::distance_` stores the distance to the center of the sun except for the moon (here distance to center of earth).
    *   - It will be a lot of copy and paste. But you can also use the vector `planets_` if you like. Note that its elements are pointers (use -> instead of .)
    * 2. Allow the system the be animated by respecting the rotational angles of all celestial bodies. Assume the following:
    *   - The sun rotates around the y-axis (up), as it is placed at the origin. (see above).
    *   - All planets rotate around the sun, i.e. around the y-axis. The current angle is stored in `Planet::angle_sun_` (except for the moon).
    *   - The moon rotates around the the earth, i.e. around a vertical axis through the earth's center. The current angle is stored in `Planet::angle_sun_`.
    *   - All planets rotate around their own axis. The current angle is stored in `Planet::angle_self_`.
     *  Hints:
     *  - All planets and the moon are stored in objects named by their name (e.g. earth_, venus_, moon_,...)
     *  - You can also use the vector `planets_` is you want to use a loop.
     *  - Use the `name_` member identify specific planets in the loop.
    * */

    mercury_.model_matrix_ = mat4::rotate_y(mercury_.angle_sun_) * mat4::translate(vec4(mercury_.distance_,0,0,1)) * mat4::rotate_y(mercury_.angle_self_)  * mat4::scale(mercury_.radius_);
    mercury_.pos_ =  (mat4::rotate_y(mercury_.angle_sun_) * mat4::translate(vec4(mercury_.distance_,0,0,1)) * mat4::scale(mercury_.radius_)) * mercury_.pos_;

    venus_.model_matrix_ = mat4::rotate_y(venus_.angle_sun_) * mat4::translate(vec4(venus_.distance_,0,0,1)) * mat4::rotate_y(venus_.angle_self_) *  mat4::scale(venus_.radius_);
    venus_.pos_ =  (mat4::rotate_y(venus_.angle_sun_) * mat4::translate(vec4(venus_.distance_,0,0,1)) * mat4::scale(venus_.radius_)) * venus_.pos_;

    earth_.model_matrix_ = mat4::rotate_y(earth_.angle_sun_) * mat4::translate(vec4(earth_.distance_,0,0,1)) * mat4::rotate_y(earth_.angle_self_) * mat4::scale(earth_.radius_);
    earth_.pos_ =  (mat4::rotate_y(earth_.angle_sun_) * mat4::translate(vec4(earth_.distance_,0,0,1)) * mat4::scale(earth_.radius_)) * earth_.pos_;

    moon_.model_matrix_ = mat4::rotate_y(earth_.angle_sun_) * mat4::translate(vec4(earth_.distance_,0,0,1)) * mat4::rotate_y(moon_.angle_sun_) * mat4::translate(vec4(moon_.distance_,0,0,1)) * mat4::rotate_y(moon_.angle_self_) * mat4::scale(moon_.radius_);
    moon_.pos_ =  (mat4::rotate_y(moon_.angle_sun_) * mat4::translate(vec4(moon_.distance_,0,0,1)) * mat4::scale(moon_.radius_)) * moon_.pos_;

    mars_.model_matrix_ = mat4::rotate_y(mars_.angle_sun_) * mat4::translate(vec4(mars_.distance_,0,0,1)) * mat4::rotate_y(mars_.angle_self_) * mat4::scale(mars_.radius_);
    mars_.pos_ =  (mat4::rotate_y(mars_.angle_sun_) * mat4::translate(vec4(mars_.distance_,0,0,1)) * mat4::scale(mars_.radius_)) * mars_.pos_;

    jupiter_.model_matrix_ =  mat4::rotate_y(jupiter_.angle_sun_) * mat4::translate(vec4(jupiter_.distance_,0,0,1)) * mat4::rotate_y(jupiter_.angle_self_) * mat4::scale(jupiter_.radius_);
    jupiter_.pos_ =  (mat4::rotate_y(jupiter_.angle_sun_) * mat4::translate(vec4(jupiter_.distance_,0,0,1)) * mat4::scale(jupiter_.radius_)) * jupiter_.pos_;

    saturn_.model_matrix_ =  mat4::rotate_y(saturn_.angle_sun_) * mat4::translate(vec4(saturn_.distance_,0,0,1)) * mat4::rotate_y(saturn_.angle_self_) * mat4::scale(saturn_.radius_);
    saturn_.pos_ =  (mat4::rotate_y(saturn_.angle_sun_) * mat4::translate(vec4(saturn_.distance_,0,0,1)) * mat4::scale(saturn_.radius_)) * saturn_.pos_;

}

//-----------------------------------------------------------------------------


void Solar_viewer::draw_scene(mat4& _projection, mat4& _view)
{
	// the matrices we need: model, modelview, modelview-projection, normal
	mat4 m_matrix;
	mat4 mv_matrix;
	mat4 mvp_matrix;
	mat3 n_matrix;

	// the sun is centered at the origin and -- for lighting -- considered to be a point, so that is the light position in world coordinates
	vec4 light = vec4(0.0, 0.0, 0.0, 1.0); //in world coordinates
	// convert light into camera coordinates
	light = _view * light;


	/** \todo Implement a rotating solar system.
    * 1. Implement the function `Solar_viewer::update_planets_positions()`.
    *
    * 2. Complete `Solar_viewer::paint()` and `Solar_viewer::keyboard(...)` to activate camera movement
    *    and render the `stars_` to have a nicer background (this will be similar to rendering the sun).
    *
    * 3. Once you have placed all celestial bodies, you can implement the various shading models.
    *    - Start with the phong shader (`phong.vert`, `phong.frag`).
    *    - As soon as that works, continue with the earth shader (`earth.vert`, `earth.frag`).
    *
    * 4. If you completed `Ship::load_model(...)`, render it with your phong shader.
    *
    * 5. Change the planets' distances. In `Solar_viewer::Solar_viewer()` you find a second version of positions.
    *
    * 6. To get a glowing sun aura, we use a billboard. That is a simple quad texture always facing the camera.
    *    - Complete `Texture::createSunBillboard()` and render it with the color shader.
    *    - Complete `Billboard::computeBillboardAngles(...)` and use `sunglow_.angle_x_` and sunglow_.angle_y_` to rotate your billboard.
    *    - Because the sunglow uses transparency you will have to use `glEnable(GL_BLEND)` and `glDisable(GL_BLEND)` to activate and deactivate blending.
    *    - The order, in which the objects are rendered is also important if you use transparency.
    *
    * 7. (Bonus task) Add saturn and its rings.
    *    - Complete `Saturn_Rings::initGlArrays()`.
    *    - Render the semitransparent rings using our `color_shader_` and alpha blending.
    *    - Saturn's rotation axis should be tilted and perpendicular to the ring's plane.
    *    - Make sure that the rings are visible behind the sunglow and vice versa by choosing the draw order of both to be dependent on which one is nearer to the camera.
    *
    * 8. (optional) Are you bored by your static sunmodel? In `color.vert` you could use sine and cosine functions to dislocate your vertices (and/or texture coordinates) slightly.
    *    If you add a time parameter as uniform to the shader, this can mimic a nice flow effekt.
	*/

    // render sun with simple shader
    m_matrix = sun_.model_matrix_;
    mv_matrix = _view * m_matrix;
    mvp_matrix = _projection * mv_matrix;
    color_shader_.use();
    color_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    color_shader_.set_uniform("tex", 0);
    color_shader_.set_uniform("greyscale", (int)greyscale_);
    sun_.draw();

    /*phong_shader_.use();
    phong_shader_.set_uniform("light_position",light);
    phong_shader_.set_uniform("tex", 0);
    phong_shader_.set_uniform("greyscale", (int)greyscale_);

    ship_.draw();

    phong_shader_.set_uniform("modelview_matrix",mv_matrix);
    phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    phong_shader_.set_uniform("normal_matrix",n_matrix);

    ship_.draw();
*/
    for(Planet* planet : planets_)
    {
        m_matrix = planet->model_matrix_;
        mv_matrix  = _view * m_matrix;
        mvp_matrix = _projection * mv_matrix;
/*
        phong_shader_.set_uniform("modelview_matrix",mv_matrix);
        phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
        phong_shader_.set_uniform("normal_matrix",n_matrix);
        */
        //color_shader_.set_uniform("modelview_matrix",mv_matrix);
        color_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
       // color_shader_.set_uniform("normal_matrix",n_matrix);
        planet->draw();
    }


    // check for OpenGL errors
    glCheckError();
}

void Solar_viewer::randomize_planets()
{
    std::cout << "Randomizing planets..." << std::endl;
    float temp_dt = time_step_;
    time_step_ = (float)(rand()%2000000);
    timer();
    time_step_ = temp_dt;
}


//=============================================================================
