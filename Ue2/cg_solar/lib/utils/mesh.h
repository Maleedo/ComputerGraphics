//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#pragma once
//=============================================================================

#include "gl.h"
#include "glmath.h"
#include <vector>

//=============================================================================

class Mesh
{
public:

    /// default constructor
    Mesh();

    /// destructor
    ~Mesh();

    /// render mesh
    virtual void draw(GLenum mode = GL_TRIANGLES) = 0;


protected:

    /// generate sphere vertices/triangles and OpenGL buffers
    void initialize(GLfloat *positions, GLfloat *normals, GLfloat *texcoords, GLuint  *indices, int n_triangles, int n_vertices, int n_indices);

    /// wrapper function for initialize to use std::vector
    bool initialize(const std::vector<vec3> &positions, const std::vector<vec3> &normals, const std::vector<float> &u_coordinates, const std::vector<float> &v_coordinates, const std::vector<unsigned int> &indices);

protected:

    /// indices of the triangle vertices
    unsigned int n_indices_;

    // vertex array object
    GLuint vao_;
    /// vertex buffer object
    GLuint vbo_;
    /// normals buffer object
    GLuint nbo_;
    /// texture coordinates buffer object
    GLuint tbo_;
    /// index buffer object
    GLuint ibo_;
};
