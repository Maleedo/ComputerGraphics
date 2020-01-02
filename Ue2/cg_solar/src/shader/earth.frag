//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 150

in vec3 v2f_normal;
in vec2 v2f_texcoord;
in vec3 v2f_light;
in vec3 v2f_view;

out vec4 f_color;

uniform sampler2D day_texture;
uniform sampler2D night_texture;
uniform sampler2D cloud_texture;
uniform sampler2D gloss_texture;
uniform bool greyscale;

const float shininess = 20.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main()
{
    /** \todo Implement a fancy earth shader.
    * - Copy your working code from the fragment shader of your Phong shader use it as starting point
    * - instead of using a single texture, use the four texures `day_texure`, `night_texure`,
    *   `cloud_texure` and `gloss_texture` and mix them for enhanced effects:
    *       * The `gloss_texture` defines how glossy (specular) a point on earth is
    *       * The `cloud_texture` defines how cloudy a point on earth is, and more clouds should produce less reflections at this point
    *       * `day_texture` and `night_texture` define the day and night color of the earth. Make sure that there is a soft transition between both.
    * Hints: 
    *   - cloud and gloss textures are just greyscales. So you'll just need one color component.
    *   - The texture(texture, 2d_position) returns a 4-vector (rgba). You can use either `texture(...).r` to get just the red component
    *     or `texture(...).rgb` to get a vec3 color value
    *   - use mix(vec3 a,vec3 b, s) = a*(1-s) + b*s for linear interpolation of two colors
    *   - There is not the one right way to get the desired results. Feel free to use some magic numbers or creative solutions.
     */
    
/**
    // fetch color from texture
    vec3 material = texture(day_texture, v2f_texcoord.st).rgb;
    float alpha = texture(tex, v2f_texcoord.st).a;

    vec3 color = vec3(0.0,0.0,0.0);

    vec3 ambient_color = material * sunlight * 0.2;

    float light_angle = dot(v2f_light,v2f_normal);

    vec3 diffuse_color = vec3(0,0,0);

    //if(light_angle > 0) {
        diffuse_color = material * sunlight * light_angle;
    //}

    vec3 specular_color = material * sunlight * pow((dot(reflect(v2f_light,v2f_normal),v2f_view)),shininess) * 0.2;

    color += ambient_color + diffuse_color + specular_color;


    // convert RGB color to YUV color and use only the luminance
    if (greyscale) color = vec3(0.299*color.r+0.587*color.g+0.114*color.b);

    // add required alpha value
    f_color = vec4(color, alpha);

    **/
}
