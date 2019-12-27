#include "billboard.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include <algorithm>

Billboard::Billboard(float _size)
    : Mesh(), size_(_size), angle_x_(0), angle_y_(0)
{}

Billboard::~Billboard()
{
}

void Billboard::draw(GLenum mode)
{
    if(n_indices_ == 0) initGlArrays();

    tex_.bind();

	glBindVertexArray(vao_);
    glDrawElements(mode, n_indices_, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

bool Billboard::initGlArrays()
{
    std::cout << "creating billboard " << std::endl;

    GLfloat *positions = new GLfloat[3 * 4];
    GLuint  *indices = new GLuint[3 * 2];
    GLfloat *tex_coords = new GLfloat[2 * 4];

    // generate vertices
    positions[0] = -1.0f;
    positions[1] = 1.0f;
    positions[2] = 0.0f;

    tex_coords[0] = 0.0f;
    tex_coords[1] = 1.0f;

    positions[3 + 0] = 1.0f;
    positions[3 + 1] = 1.0f;
    positions[3 + 2] = 0.0f;

    tex_coords[2 + 0] = 1.0f;
    tex_coords[2 + 1] = 1.0f;

    positions[6 + 0] = -1.0f;
    positions[6 + 1] = -1.0f;
    positions[6 + 2] = 0.0f;

    tex_coords[4 + 0] = 0.0f;
    tex_coords[4 + 1] = 0.0f;

    positions[9 + 0] = 1.0f;
    positions[9 + 1] = -1.0f;
    positions[9 + 2] = 0.0f;

    tex_coords[6 + 0] = 1.0f;
    tex_coords[6 + 1] = 0.0f;

    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 1;

    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;

    initialize(positions, positions, tex_coords, indices, 2, 4, 6);

    // clean up
    delete[] positions;
    delete[] tex_coords;
    delete[] indices;

    return true;
}

void Billboard::computeBillboardAngles(const vec3 &billboard_to_eye)
{
    /** \todo update `angle_x_` and `angle_y_` of the billboard.
    *   - Make sure that the vector `billboard_to_eye` is always perpendicular to the billboard's plane
    *	- The two angles are related to the polar and azimuthal angle of a sphere (see https://en.wikipedia.org/wiki/Spherical_coordinate_system)
    *   - https://neutrium.net/mathematics/converting-between-spherical-and-cartesian-co-ordinate-systems/ shows how to convert cartesian coordinates to spherical ones.
    *   - The function `atan2(...)` is already defined.
    *   - There is an offset between the spherical angles and the needed angles. Try different 90° offsets until you get the desired result.
    *   Hint:
    *       It helps to set the look_at to the sun (key 1) and print `Solar_viewer::x_angle_` and `Solar_viewer::y_angle_` to find the right offset.
    *       In this case, those angles should be identical to `angle_x_` and `angle_y_`.
    **/


}

