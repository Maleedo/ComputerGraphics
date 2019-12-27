//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#ifndef PLANET_H
#define PLANET_H
//=============================================================================

#include "texture.h"
#include "glmath.h"
#include "sphere.h"

//=============================================================================


/// default class for the planet
class Planet
{
public:

    /// default constructor
    Planet(const std::string &name,
           const float _angle_step_sun,
           const float _angle_step_self,
           const float _radius,
           const float _distance) :
        pos_(_distance,0,0,1),
        distance_(_distance),
        radius_(_radius),
        angle_sun_(0),
        angle_self_(0),
        angle_step_sun_(_angle_step_sun),
        angle_step_self_(_angle_step_self),
        model_matrix_(mat4::scale(0.5f)),
        name_(name)
    {}

    /// set the time for every update
    void time_step(float _days)
    {
        angle_sun_  += _days * angle_step_sun_;
        angle_self_ += _days * angle_step_self_;
    }

    void draw()
    {
        tex_.bind();
        unit_sphere_mesh_.draw();
    }

    /// the current position of the planet
    vec4 pos_;
    /// distance of the planet from the sun (=origin)
    float distance_;
    /// the radius of the planet
    float radius_;

    /// current rotation around the sun
    float angle_sun_;
    /// current rotation around planet center
    float angle_self_;

    /// how much to rotation around the sun in one day (in radians)
    float angle_step_sun_;
    /// how much to rotation around planet center in one day (in radians)
    float angle_step_self_;

    static Sphere unit_sphere_mesh_;

    /// main diffuse texture for the planet
    Texture tex_;

    /// planet's model matrix
    mat4 model_matrix_;

    /// planet's name
    std::string name_;
};


//=============================================================================


/// earth is a special planet, because we need access to more textures
class Earth : public Planet
{
public:

    /// default constructor for the earth
    Earth(const std::string &name,
          const float _update,
          const float _update_self,
          const float _scale,
          const float _distance)
        :Planet(name, _update, _update_self,_scale, _distance){}

    /// diffuse texture for the night
    Texture night_;

    /// texture with the clouds
    Texture cloud_;

    /// the gloss texture - defines where the surface si glossy or not
    Texture gloss_;

    /// the normal texture - detailed normal maps
    Texture normal_;
};


//=============================================================================
#endif // PLANET_H
//=============================================================================
