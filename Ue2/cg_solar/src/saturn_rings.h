#ifndef SATURNRINGS_H
#define SATURNRINGS_H

#include "glmath.h"
#include <vector>
#include "texture.h"
#include "mesh.h"

class Saturn_Rings : public Mesh
{
public:
    Saturn_Rings(float _radius1, float _radius2, int _resolution)
        : Mesh(), inner_radius_(_radius1), outer_radius_(_radius2), resolution_(_resolution){}

    ~Saturn_Rings(){}

    /// init OpenGL arrays
    bool initGlArrays()
    {
        std::cout << "creating saturn rings..." << std::endl;

        std::vector<vec3> positions;
        std::vector<vec3> normals;
        std::vector<unsigned int> indices;
        std::vector<float> u_coords;
        std::vector<float> v_coords;

        /** \todo (Bonus Task) Define the saturn ring mesh.
         *    - Fill the vectors defined above. Therefore, find a way to define a ring by triangles.
         *    - `inner_radius_` and `outer_radius_` define the size of the ring.
         *    - `resolution_` should determine how many vertices build the outer and inner boundary.
         *      So, if you decrease `resolution_` to 3 for example, the ring should look like a triangle with a hole in the middle.
         *    - `textures/saturn_rings.png` is the texture we use for the ring.
         *      Think about how you have to define the texcoords `u_coords` and `v_coords` to get the desired effect.
         *
         * Hints: - If you use the color shader to render the rings in `Solar_Viewer::draw_scene(..)` the normal vectors are not used.
         *      So they can hold arbitrary values in this case.
         *        - To test your rings' vertices without worrying about texture coordinates, you can simply use `u = v = 0.5` for each vertex.
         */


        if(!initialize(positions, normals, u_coords, v_coords, indices))
        {
            std::cerr << "\nError: Saturn rings cannot be initialized!\n" << std::endl;
            return false;
        }

        return true;
    }

    /// render the billboard
    void draw(GLenum mode = GL_TRIANGLES) override
    {
        static bool first = true;
        if(n_indices_ == 0 && first)
        {
            initGlArrays();
            first = false;
        }

        tex_.bind();

        glBindVertexArray(vao_);
        glDrawElements(mode, n_indices_, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
    }

    /// size of our rings
    float inner_radius_, outer_radius_;

    /// tesselation resolution of the rings (number of slices of the ring)
    int resolution_;

    /// saturn ring texture
    Texture tex_;

};

#endif // SATURNRINGS_H
