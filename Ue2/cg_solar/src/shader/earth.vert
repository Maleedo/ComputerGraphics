//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec4 v_position; // input vertex position
layout (location = 1) in vec3 v_normal;   // input vertex normal
layout (location = 2) in vec2 v_texcoord; // input texture coordinates

out vec3 v2f_normal;    // output: normal at current point
out vec3 v2f_light;     // output: normalized light vector from current vertex to light source
out vec3 v2f_view;      // output: normalized view vector from current vertex to the eye
out vec2 v2f_texcoord;  // output: texture coordinates

uniform mat4 modelview_projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform vec4 light_position; //in eye space coordinates already



void main()
{
    /** \todo 
     * - Copy your working code from the vertex shader of your phong shader.
     *   and continue with the fragment shader
     */
/*
    v2f_texcoord = v_texcoord;

    gl_Position = modelview_projection_matrix * v_position;
    v2f_normal = normal_matrix * (modelview_matrix * vec4(v_normal,0)).xyz;
    vec4 v2f_v_position_cam = modelview_matrix * v_position;
    v2f_light = normalize(light_position - v2f_v_position_cam).xyz;
    v2f_view = normalize(-(v2f_v_position_cam)).xyz;
*/

} 
