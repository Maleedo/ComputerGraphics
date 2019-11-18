//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================


//== INCLUDES =================================================================

#include "Plane.h"
#include <float.h>



//== CLASS DEFINITION =========================================================



Plane::Plane(const vec3& _center, const vec3& _normal)
: center(_center), normal(_normal)
{
}


//-----------------------------------------------------------------------------

bool
Plane::
intersect(const Ray& _ray,
          vec3&      _intersection_point,
          vec3&      _intersection_normal,
          vec3&      _intersection_diffuse,
          double&    _intersection_t ) const
{

    _intersection_diffuse = material.diffuse;

/** \to do
 * - compute the intersection of the plane with `_ray`
 * - if ray and plane are parallel there is no intersection
 * - otherwise compute intersection data and store it in `_intersection_point`, `_intersection_normal`, and `_intersection_t`.
 * - return whether there is an intersection for t>1e-5 (avoids "shadow acne").
*/

    _intersection_t = dot(normal, center - _ray.origin)/dot(_ray.direction, normal);

    //Avoid shadow acne

    if (distance(_ray.origin, _ray(_intersection_t)) > 0.00001)
    {
    	_intersection_point = _ray(_intersection_t);
        _intersection_normal = normal;
        return(_intersection_t >= 0);
    }
    return false;
}


//=============================================================================
