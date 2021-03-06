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

#include "Mesh.h"
#include "Plane.h"
#include <math.h>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <algorithm>
#include <iterator>

//== IMPLEMENTATION ===========================================================


Mesh::Mesh(Draw_mode _draw_mode, std::string _filename)
{
    // set draw mode
    draw_mode_ = _draw_mode;

    hasTexture_ = false;

    // load mesh from file
    read_obj(_filename.c_str());

}


bool Mesh::read_obj(const char* _filename)
{
    // open obj file
    std::ifstream ifs(_filename);
    if (!ifs)
    {
        std::cerr << "Can't open " << _filename << "\n";
        return false;
    }

    bool hasNormals = false;
    bool hasUV = false;

    std::string filename(_filename);
    std::string line;
    int counter = -1;
    std::map<int, bool> uvDone;
    std::vector<Image> textures;
    // parse line by line
    while(std::getline(ifs,line))
    {
        //devide line into header (first word) and lineData (rest)
        size_t firstSpace = line.find_first_of(" ");
        std::string header = line.substr(0,firstSpace);
        std::istringstream lineData(line.substr(firstSpace + 1));

        //vertices
        if(header == "v")
        {
            Vertex v;
            lineData >> v.position[0] >> v.position[1] >> v.position[2];

            vertices_.push_back(v);
            continue;
        }

        //uv-coordinates
        if (header == "vt")
        {
            hasUV = true;

            double u,v;

            lineData >> u >> v;

            if(u > 1.0 || u < 0.0)
                u -= floor(u);
            if(v > 1.0 || v < -0.0)
                v -= floor(v);

            u_coordinates_.push_back(u);
            v_coordinates_.push_back(v);
            continue;
        }

        if (header == "vn")
        {
            hasNormals = true;
            continue;
        }

        // material file
        if(header == "mtllib")
        {
            std::stringstream mtl;
            mtl << filename.substr(0, filename.find_last_of("/") + 1) << lineData.str();

            if (!read_mtl(mtl.str(), textures))
            {
                std::cerr << "Cannot read mtl file " << mtl.str() << std::endl;
            }

            if(textures.size() > 0)
                hasTexture_ = true;

            continue;
        }

        // start of new material
        if(header == "usemtl")
        {
            counter++;
            continue;
        }

        // faces
        if (header == "f")
        {
            Triangle t;

            int uv[3];

            enum{NORMALS, UV, BOTH, NONE} nuv_status;
            if(hasUV)
                nuv_status = hasNormals ? BOTH : UV;
            else
                nuv_status = hasNormals ? NORMALS : NONE;

            // dummy varaibles for / and normal indices
            int d1;
            char d2;

            // read in face indices and uv indices, skip normal indices
            switch(nuv_status)
            {
            case BOTH:
                // format: index0/texture0/normal0 index1/texture1/normal1 index2/texture2/normal2
                lineData >> t.i0 >> d2 >> uv[0] >> d2 >> d1;
                lineData >> t.i1 >> d2 >> uv[1] >> d2 >> d1;
                lineData >> t.i2 >> d2 >> uv[2] >> d2 >> d1;

                uv[0]--; uv[1]--; uv[2]--;
                t.iuv0 = uv[0]; t.iuv1 = uv[1]; t.iuv2 = uv[2];
                break;
            case UV:
                // format: index0/texture0 index1/texture1 index2/texture2
                lineData >> t.i0 >> d2 >> uv[0];
                lineData >> t.i1 >> d2 >> uv[1];
                lineData >> t.i2 >> d2 >> uv[2];

                uv[0]--; uv[1]--; uv[2]--;
                t.iuv0 = uv[0]; t.iuv1 = uv[1]; t.iuv2 = uv[2];
            case NORMALS:
                // format: index0//normal0 index1//normal1 index2//normal2
                lineData >> t.i0 >> d2 >> d2 >> d1;
                lineData >> t.i1 >> d2 >> d2 >> d1;
                lineData >> t.i2 >> d2 >> d2 >> d1;
            case NONE:
                // format: index0 index1 index2
                lineData >> t.i0 >> t.i1 >> t.i2;
            }

            //decrement because obj indices start by 1
            t.i0--; t.i1--; t.i2--;


            //convert uv coordinates s.th. we can use just one big combined tex instead of multiple ones
            for(int i = 0; i < 3 && hasUV; i++)
            {
                if(!uvDone[uv[i]])
                {
                    int combinedW = 0;
                    for(int i = 0; i < counter; i++)
                    {
                        combinedW += textures[i].width();
                    }
                    u_coordinates_[uv[i]] = (u_coordinates_[uv[i]]*textures[counter].width() + combinedW)/static_cast<double>(texture_.width());
                    v_coordinates_[uv[i]] = (v_coordinates_[uv[i]]*textures[counter].height())/static_cast<double>(texture_.height());
                    uvDone[uv[i]] = true;
                }
            }

            // add triangle to our triangle vector
            triangles_.push_back(t);
        }
    }

    std::cout << "\n  read " << _filename << ": " << vertices_.size() << " vertices, " << triangles_.size() << " triangles" << std::flush;

    compute_bounding_box();
    compute_normals();

    return true;
}

//-----------------------------------------------------------------------------

bool Mesh::read_mtl(std::string path, std::vector<Image> &textures){

    // open mtl file
    std::ifstream ifs(path.c_str());
    if (!ifs)
    {
        std::cerr << "Can't open " << path << "\n";
        return false;
    }

    std::string line;
    int numTexturesPerMaterial = 0;
    Image tmpimage;

    // parse line by line
    while(std::getline(ifs,line))
    {
        //devide line into header (first word) and lineData (rest)
        size_t firstSpace = line.find_first_of(" ");
        std::string header = line.substr(0,firstSpace);
        std::istringstream lineData(line.substr(firstSpace + 1));

        if(header == "newmtl")
        {
            numTexturesPerMaterial = 0;
            continue;
        }

        if(header.substr(0,3) == "map" && numTexturesPerMaterial == 0)
        {
            std::stringstream tmp;
            tmp << path.substr(0,path.find_last_of("/") + 1) << lineData.str();

            tmpimage.read(tmp.str().c_str());
            textures.push_back(tmpimage);
            numTexturesPerMaterial++;
        }
    }

    unsigned int maxH = 0;
    unsigned int sumW = 0;
    for(int i = 0; i < textures.size();i++)
    {
        sumW += textures[i].width();
        maxH = std::max(maxH, textures[i].height());
    }
    texture_.resize(sumW, maxH);


    for(int x = 0; x < sumW; x++)
    {
        for(int y = 0; y < maxH; y++)
        {
            unsigned int texnr = 0;
            unsigned int combinedW = 0;

            for(int i = 0; i < textures.size();i++)
            {
                if(x >= combinedW + textures[i].width())
                {
                    combinedW += textures[i].width();
                    texnr++;
                }
                else
                    break;
            }

            if(y < textures[texnr].height())
            {
                texture_(x,y) = textures[texnr](x - combinedW,y);
            }
            else
            {
                texture_(x,y) = vec3(0,0,0);
            }

        }
    }

    return true;

}

//-----------------------------------------------------------------------------

void Mesh::compute_normals()
{
    /** \todo
     * In some scenes (e.g the office scene) some objects have to be flat
     * shaded (e.g. the desk) while other objects should be Phong shaded to appear
     * realistic (e.g. chairs). We provide code to compute triangle normals. You have to
     * implement the following:
     * - Compute vertex normals by averaging the normals of their incident triangles.
     * - Store the vertex normals in the Vertex::normal member variable.
     * - Weigh the normals by their triangles' angles.
     */


    // compute triangle normals
    for (Triangle& t: triangles_)
    {
        const vec3& p0 = vertices_[t.i0].position;
        const vec3& p1 = vertices_[t.i1].position;
        const vec3& p2 = vertices_[t.i2].position;
        t.normal = normalize(cross(p1-p0, p2-p0));
    }

    //Compute vertex normal for every vertex v in the mesh
    for (Vertex& v: vertices_)
    {
		vec3 v_pos = v.position;

		// Sum of weighted triangle normals for vertex normal equation
        vec3 weighted_vector_sum = {0,0,0}; //Sum over all weighted triangle normals connected to that vertex

        //Check every triangle in the mesh and compare its vertices to v.
        //If a match is found, compute opening angle (used as weight) and use triangle normal for vertex normal
        //computation for v.
        for(Triangle& t: triangles_){

            //Store triangle vertices
			const vec3& p0 = vertices_[t.i0].position;
			const vec3& p1 = vertices_[t.i1].position;
			const vec3& p2 = vertices_[t.i2].position;

            //Check if v is part of triangle
			if(v_pos == p0){

			    //Compute adjacent vectors of triangle vertix
				vec3 adjacent_1 = p1 - p0;
				vec3 adjacent_2 = p2 - p0;

				//Opening angle used as weight
				double opening_angle = acos(dot(normalize(adjacent_1), normalize(adjacent_2))) ;

				//Add weighted triangle normal to vector_sum
				weighted_vector_sum += opening_angle * t.normal;
			}

			if(v_pos == p1){
				vec3 adjacent_1 = p2 - p1;
				vec3 adjacent_2 = p0 - p1;
				double opening_angle = acos(dot(normalize(adjacent_1), normalize(adjacent_2)));
				weighted_vector_sum += opening_angle * t.normal;
			}

			if(v_pos == p2){
				vec3 adjacent_1 = p0 - p2;
				vec3 adjacent_2 = p1 - p2;
				double opening_angle = acos(dot(normalize(adjacent_1), normalize(adjacent_2)));
				weighted_vector_sum += opening_angle * t.normal;
			}

		}

		//Compute actual vertex normal
		v.normal = normalize(weighted_vector_sum);

    }
}


//-----------------------------------------------------------------------------


void Mesh::compute_bounding_box()
{
    bb_min_ = vec3( DBL_MAX,  DBL_MAX,  DBL_MAX);
    bb_max_ = vec3(-DBL_MAX, -DBL_MAX, -DBL_MAX);

    for (Vertex v: vertices_)
    {
        bb_min_ = min(bb_min_, v.position);
        bb_max_ = max(bb_max_, v.position);
    }
}


//-----------------------------------------------------------------------------


bool Mesh::intersect_bounding_box(const Ray& _ray) const
{
    /** \todo
    * Intersect the ray `_ray` with the axis-aligned bounding box of the mesh.
    * Note that the minimum and maximum point of the bounding box are stored
    * in the member variables `bb_min_` and `bb_max_`. Return whether the ray
    * intersects the bounding box.
    *
    * Hints:
    * - The box intersection is basically a combination of 6 plane/ray intersections.
    * - The resulting intersection point has to be checked against the lower and upper bounds `bb_min_` and `bb_max_`.
    * - The intersection tests can be done easier than the existing general ray/plane intersection because the planes are axis aligned.
    * - One positive intersection is sufficient to return true.
    * - It helps to visualize a 2D ray/rectangle intersection on a sheet of paper.
    * - To debug your bounding box code, comment out your triangle intersection test (return true at the beginning) and test
    *   the cube or toon_faces scene. You should see the black bounding boxes, if everything is correct.
    * - There are other (faster) ray/box intersection approaches, feel free to implement one of those instead.
    *
    * Note:
    * This function is used in `Mesh::intersect()` to avoid the intersection test
    * with all triangles of every mesh in the scene. The bounding boxes are computed
    * in `Mesh::compute_bounding_box()`.
    */


    //Using algorithm for ABBs from cg-script
    double t_min = 0;
    double t_max = DBL_MAX;

    bool intersect_Min_Plane;
    bool intersect_Max_Plane;

    //Creating planes of bounding box
    for(int i = 0; i<3; i++){
        vec3 axis_normal = (0,0,0);
        axis_normal[i] += 1;

        Plane Min_Plane = Plane(bb_min_, axis_normal);
        Plane Max_Plane = Plane(bb_max_, axis_normal);

        vec3 intersection_point;
        vec3 intersection_normal;
        vec3 intersection_diffuse;
        double intersection_t_min;
        double intersection_t_max;

        //Intersecting ray with 2 parallel planes
        intersect_Min_Plane = Min_Plane.intersect(_ray, intersection_point, intersection_normal, intersection_diffuse, intersection_t_min);
        intersect_Max_Plane = Max_Plane.intersect(_ray, intersection_point, intersection_normal, intersection_diffuse, intersection_t_max);

        //Ensuring that intersection_t_min < intersection_t_max and updating t_min and t_max
        if(intersection_t_min < intersection_t_max){
                t_min = std::max(t_min, intersection_t_min);
                t_max = std::min(t_max, intersection_t_max);
        }else {
                t_min = std::max(t_min, intersection_t_max);
                t_max = std::min(t_max, intersection_t_min);

        }

    //If t_min < t_max after intersecting every plane, the ray intersects with the bounding box
    }
    if(t_min < t_max){
        return true;
    }else {
        return false;
    }
}


//-----------------------------------------------------------------------------


bool Mesh::intersect(const Ray& _ray,
                     vec3&      _intersection_point,
                     vec3&      _intersection_normal,
                     vec3&      _intersection_diffuse,
                     double&    _intersection_t ) const
{
    // check bounding box intersection
    if (!intersect_bounding_box(_ray))
    {
        return false;
    }

    vec3   p, n, d;
    double t;

    _intersection_t = DBL_MAX;

    // for each triangle
    for (const Triangle& triangle : triangles_)
    {
        // does ray intersect triangle?
        if (intersect_triangle(triangle, _ray, p, n, d, t))
        {

            // is intersection closer than previous intersections?
            if (t < _intersection_t)
            {
                // store data of this intersection
                _intersection_t      = t;
                _intersection_point  = p;
                _intersection_normal = n;
                _intersection_diffuse= d;
            }
        }
    }

    return (_intersection_t != DBL_MAX);
}


//-----------------------------------------------------------------------------

//Function declarations of math functions used in cramers rule. Function definitions can be found under Mesh::intersect_triangle.

void replace_column(const int column_number, const vec3 _iter_column,const double _coefficient_matrix[3][3], double (&_divisor_matrix)[3][3]);
double determinant(const double matrix[][3]);
double cramer(double &_intersection_t, double &_beta, double &_gamma, const double _coefficient_matrix[3][3], const vec3 _iter_column);


bool
Mesh::
intersect_triangle(const Triangle&  _triangle,
                   const Ray&       _ray,
                   vec3&            _intersection_point,
                   vec3&            _intersection_normal,
                   vec3&            _intersection_diffuse,
                   double&          _intersection_t) const
{
    _intersection_diffuse = material.diffuse;

    /** \todo
    * Intersect _ray with _triangle:
    * - store intersection point in `_intersection_point`
    * - store ray parameter in `_intersection_t`
    * - store normal at intersection point in `_intersection_normal`.
    * - Depending on the member variable `draw_mode_`, use either the triangle
    *  normal (`Triangle::normal`) or interpolate the vertex normals (`Vertex::normal`).
    * - return `true` if there is an intersection with t > 1e-5 (avoid "shadow acne")
    *
    * Hint: Rearrange `ray.origin + t*ray.dir = a*p0 + b*p1 + (1-a-b)*p2` to obtain a solvable
    * system for a, b and t.
    * Refer to [Cramer's Rule](https://en.wikipedia.org/wiki/Cramer%27s_rule) to easily solve it.
     */

    const vec3& p0 = vertices_[_triangle.i0].position;      // Point (vertex) of triangle as vector
    const vec3& p1 = vertices_[_triangle.i1].position;
    const vec3& p2 = vertices_[_triangle.i2].position;

    /*  Using explicit representation of a triangle with barycentric coordinates to determine any point on the triangle.
        Creating linear system out of vector coordinates to solve for t, beta and gamma with alpha = 1-beta-gamma.
    */

    //columns of coefficient matrix
    vec3 first_column  = -_ray.direction;
    vec3 second_column = p1-p0;
    vec3 third_column  = p2-p0;

    //coefficient matrix
    double coefficient_matrix[3][3] = {
    {first_column[0], second_column[0], third_column[0]},
    {first_column[1], second_column[1], third_column[1]},
    {first_column[2], second_column[2], third_column[2]}
    };

    //"right" side of linear system - constant number without variables
    vec3 iter_column = _ray.origin - p0;

    //barycentric coordinates
    double beta;
    double gamma;
    double alpha = cramer(_intersection_t, beta, gamma, coefficient_matrix, iter_column);

    //if barycentric coordinates > 0, then point lies within triangle
    if(!(alpha > 0 && beta > 0 && gamma > 0)){
        return false;
    }

    //computed intersection point
    _intersection_point = _ray(_intersection_t);


    //Use flat or phong shading
    if(draw_mode_ == FLAT ){
        _intersection_normal = _triangle.normal;
    }

    if(draw_mode_ == PHONG)
    {
        //Get interpolated vertices normals
        vec3 vertex_A_normal = vertices_[_triangle.i0].normal;
        vec3 vertex_B_normal = vertices_[_triangle.i1].normal;
        vec3 vertex_C_normal = vertices_[_triangle.i2].normal;

        // Sum of interpolated normal vectors of triangle vertices (alpha, beta, gamma used as weights)
        vec3 sum_of_inormals = alpha * vertex_A_normal + beta * vertex_B_normal + gamma * vertex_C_normal;

        //Store interpolated normal in _intersection_normal
        _intersection_normal = normalize(sum_of_inormals);
    }

    /** \todo
    * Support textured triangles:
    * - `hasTexture_` indicates if the mesh is textured.
    * - Acces the three u and v texture coordinates stored in `u_coordinates` resp. `v_coordinates` via the triangles iuv indices.
    * - Interpolate the uv-coordinates using your barycentric coordinates to get the intersection point's uv.
    * - Convert the relative uv coordinates (from 0 to 1) to absolute pixel coordinates (from 0 to width/height - 1 of `texture_`)
    * - Store the resulting texture color in `_intersection_diffuse`
    * - You will notice that there will be shadows on the sky mesh in the pokemon scene.
    * Use `material.shadowable` in the `lighting(...)` function to prevent it from being shadowed.
    * (`material.shadowable` is already set to false for the sky mesh and true for all other meshes, so you don't have to set it by yourself)
     */

    if(hasTexture_){

        //Get 2d coordinates of triangle vertices
        std::vector<double> triangle_A; triangle_A.resize(2);
        std::vector<double> triangle_B; triangle_B.resize(2);
        std::vector<double> triangle_C; triangle_C.resize(2);

        triangle_A[0] = u_coordinates_[_triangle.iuv0];
        triangle_A[1] = v_coordinates_[_triangle.iuv0];

        triangle_B[0] = u_coordinates_[_triangle.iuv1];
        triangle_B[1] = v_coordinates_[_triangle.iuv1];

        triangle_C[0] = u_coordinates_[_triangle.iuv2];
        triangle_C[1] = v_coordinates_[_triangle.iuv2];

        //Convert intersection point from 3d to 2d, by interpolating 2d coordinates with barycentric coordinates.
        std::vector<double> intersection_point_uv; intersection_point_uv.resize(2);

        intersection_point_uv[0] = alpha * triangle_A[0] + beta * triangle_B[0] + gamma * triangle_C[0];
        intersection_point_uv[1] = alpha * triangle_A[1] + beta * triangle_B[1] + gamma * triangle_C[1];

        //Converting relative pixel value to actual pixel of image
        intersection_point_uv[0] = (int)round(intersection_point_uv[0] * (texture_.width() - 1));
        intersection_point_uv[1] = (int)round(intersection_point_uv[1] * (texture_.height() -1));

        //Storing texture color of intersection point in _intersection_diffuse
        _intersection_diffuse = texture_(intersection_point_uv[0], intersection_point_uv[1]);
    }

    //avoid shadow acne
    if(distance(_ray.origin, _ray(_intersection_t)) > 1e-5){
    return (_intersection_t >= 0);
    } else {
    return false;
    }


}

//computes determinant of given matrix

double determinant(const double matrix[][3]){
    double _determinant = matrix[0][0] * matrix[1][1] * matrix[2][2]
                    + matrix[0][1] * matrix[1][2] * matrix[2][0]
                    + matrix[0][2] * matrix[1][0] * matrix[2][1]
                    - matrix[0][2] * matrix[1][1] * matrix[2][0]
                    - matrix[0][0] * matrix[1][2] * matrix[2][1]
                    - matrix[0][1] * matrix[1][0] * matrix[2][2];

    return _determinant;
}

//uses cramer's rule to solve linear system.

double cramer(double &_intersection_t, double &_beta, double &_gamma, const double _coefficient_matrix[3][3], const vec3 _iter_column){

    double dividend = determinant(_coefficient_matrix);
    double divisor_t[3][3]; replace_column(0, _iter_column, _coefficient_matrix, divisor_t);
    double divisor_beta[3][3]; replace_column(1, _iter_column, _coefficient_matrix, divisor_beta);
    double divisor_gamma[3][3]; replace_column(2, _iter_column, _coefficient_matrix, divisor_gamma);

    _intersection_t = determinant(divisor_t)/dividend;
    _beta = determinant(divisor_beta)/dividend;
    _gamma = determinant(divisor_gamma)/dividend;

    return (1-_beta-_gamma);
}

//creates matrices with replaced columns for cramer's rule equation.

void replace_column(const int column_number, const vec3 _iter_column,const double _coefficient_matrix[3][3], double (&_divisor_matrix)[3][3]){

    for(int i = 0; i<3; i++){
        _divisor_matrix[i][column_number] = _iter_column[i];
        for(int j = 0; j<3; j++){
            if(j != column_number){
            _divisor_matrix[i][j] = _coefficient_matrix[i][j];
            }
        }
    }
}



//=============================================================================
