#include "ship.h"
#include <fstream>
#include <string>

Ship::Ship()
    :Mesh(), pos_(0, 0, 0, 1), direction_(0, 0, 1, 0), speed_(0), angle_(0), angular_speed_(0), radius_(0.01)
{}

Ship::~Ship()
{
}

bool Ship::load_model(const char* _filename)
{

	// implements a simple OFF reader

	// open file
	std::ifstream ifs(_filename);
	if (!ifs)
	{
		std::cerr << "Can't open " << _filename << "\n";
		return false;
	}

	// read OFF header
	std::string s;
	ifs >> s;
	if (s != "OFF")
	{
		std::cerr << "No OFF file\n";
		return false;
	}

	/** \todo Implement a simple .off reader.
	*   `.off` is a simple text-based mesh format
	*	1. open /meshes/cube.off with a text editor and try to find out how an `.off` file saves the mesh information
	*		(or look it up in the internet, but we recommand doing the former first)
	*	2. use the input filestream `ifs` to parse the file in `vertices_` and `indices` 
	*		(Maybe you should find out how to use ifstream objects first)
	*
	*	Hints:	
	*	- use cube.off for debugging and feel free to use your favourite spaceship mesh once your parser works.
	*	- 3D modelling software is capable of reading and writing `.off` files. (for example, google "blender import .off")
	**/

	// DELETE ME BEGIN
	vertices_.push_back(vec3(0,0,2));
	vertices_.push_back(vec3(1, 0, 0));
	vertices_.push_back(vec3(-1, 0, 0));
        indices_.push_back(0);
        indices_.push_back(1);
        indices_.push_back(2);
	// DELETE ME END



	// close file
	ifs.close();
    compute_normals();
    initGlArrays();

	return true;
}

void Ship::compute_normals()
{
	int nF = indices_.size()/3;
	int nV = vertices_.size();

	vertex_normals_.resize(nV, vec3(0, 0, 0));
	for (int i = 0; i<nF; i++)
	{
		vec3 a, b;
		a = vertices_[indices_[3 * i + 1]] - vertices_[indices_[3 * i + 0]];
		b = vertices_[indices_[3 * i + 2]] - vertices_[indices_[3 * i + 0]];
		face_normals_.push_back(normalize(cross(a, b)));

		vertex_normals_[indices_[3 * i + 0]] += face_normals_[i];
		vertex_normals_[indices_[3 * i + 1]] += face_normals_[i];
		vertex_normals_[indices_[3 * i + 2]] += face_normals_[i];
	}

	for (int i = 0; i<nV; i++)
	{
		vertex_normals_[i] = normalize(vertex_normals_[i]);
	}
}

void Ship::accelerate(float speedup)
{
    speed_ += speedup;
    speed_ = std::max(0.0f, std::min(0.03f,speed_));
}

void Ship::accelerate_angular(float angular_speedup)
{
    angular_speed_ += angular_speedup;
}

void Ship::update_ship()
{
    angle_ += angular_speed_;
    angular_speed_ *= 0.98;
    mat4 rot = mat4::rotate_y(angle_);
    direction_ = rot*vec4(0,0,1,0);
    pos_ += speed_*direction_;
}

void Ship::draw(GLenum mode)
{
    if (n_indices_ == 0) initGlArrays();

    tex_.bind();

	glBindVertexArray(vao_);
	glDrawElements(mode, n_indices_, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

bool Ship::initGlArrays()
{
    std::vector<float> texcoords;
    // generate texcoords
    for (unsigned int v = 0; v<vertices_.size(); ++v)
    {
            texcoords.push_back(0.5);
	}

    if(!initialize(vertices_, vertex_normals_, texcoords, texcoords, indices_))
    {
        std::cerr << "\nError: Ship cannot be initialized!\n" << std::endl;
        return false;
    }

    return true;
}
