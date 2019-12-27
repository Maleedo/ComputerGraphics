#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "glmath.h"
#include <vector>
#include "texture.h"
#include "mesh.h"

class Billboard : public Mesh
{
public:
    Billboard(float _size);
    ~Billboard();

    /// computes rotation angles for our billboard
    void computeBillboardAngles(const vec3 &billboard_to_eye);

    /// init OpenGL arrays
    bool initGlArrays();

    /// render the billboard
    void draw(GLenum mode = GL_TRIANGLES) override;

    /// rotation angles
    float angle_x_, angle_y_;

    /// size of our billboard
    float size_;

    Texture tex_;

};

#endif

