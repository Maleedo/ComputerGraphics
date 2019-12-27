//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>


//=============================================================================


Shader::Shader() :
    pid_(0), vid_(0), fid_(0), gid_(0)
{
}


//-----------------------------------------------------------------------------


Shader::~Shader()
{
    cleanup();
}


//-----------------------------------------------------------------------------


void Shader::cleanup()
{
    if (pid_) glDeleteProgram(pid_);
    if (vid_) glDeleteShader(vid_);
    if (fid_) glDeleteShader(fid_);
    if (fid_) glDeleteShader(fid_);

    pid_ = vid_ = fid_ = gid_ = 0;
}


//-----------------------------------------------------------------------------


bool Shader::load(const char* vfile, const char* ffile, const char* gfile)
{
    // cleanup existing shaders first
    cleanup();
    glCheckError();


    // create program
    pid_ = glCreateProgram();
    glCheckError();

    // vertex shader
    vid_ = load_and_compile(vfile, GL_VERTEX_SHADER);
    glCheckError();
    if (vid_)  glAttachShader(pid_, vid_);
    glCheckError();

    // fragment shader
    fid_ = load_and_compile(ffile, GL_FRAGMENT_SHADER);
    if (fid_)  glAttachShader(pid_, fid_);

    glCheckError();
    // geometry shader
    if (gfile)
    {
        gid_ = load_and_compile(gfile, GL_GEOMETRY_SHADER);
        if (gid_)  glAttachShader(pid_, gid_);
    }


    // link program
    glLinkProgram(pid_);
    GLint status;
    glGetProgramiv(pid_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(pid_, GL_INFO_LOG_LENGTH, &length);

        GLchar *info = new GLchar[length+1];
        glGetProgramInfoLog(pid_, length, NULL, info);
        std::cerr << "Shader: Cannot link program:\n" << info << std::endl;
        delete[] info;

        cleanup();

        return false;
    }
    glCheckError();

    return true;
}


//-----------------------------------------------------------------------------


GLint Shader::load_and_compile(const char* filename, GLenum type)
{
    // read file to string
    std::ifstream  ifs(filename);
    if (!ifs)
    {
        std::cerr << "Shader: Cannot open file \""  << filename << "\"\n";
        return 0;
    }
    std::stringstream  ss;
    ss << ifs.rdbuf();
    std::string str = ss.str();
    const char* source = str.c_str();
    ifs.close();


    // create shader
    GLint id = glCreateShader(type);
    if (!id)
    {
        std::cerr << "Shader: Cannot create shader object for \""  << filename << "\"\n";
        return 0;
    }


    // compile vertex shader
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);


    // check compile status
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        GLchar *info = new GLchar[length+1];
        glGetShaderInfoLog(id, length, NULL, info);

        std::cerr << "Shader: Cannot compile shader \""  << filename << "\"\n" << info << std::endl;

        delete[] info;
        glDeleteShader(id);

        return 0;
    }


    return id;
}


//-----------------------------------------------------------------------------


void Shader::use()
{
    if (pid_) glUseProgram(pid_);
}


//-----------------------------------------------------------------------------


void Shader::disable()
{
    glUseProgram(0);
}

//-----------------------------------------------------------------------------


void Shader::set_uniform(const char* name, bool value)
{
    if (!pid_) return;
    int location = glGetUniformLocation(pid_, name);
    if (location == -1) {
        std::cerr<<"Invalid uniform location for: "<<name<<std::endl;
        return;
    }
    glUniform1i(location, static_cast<int>(value));
    return;
}

//-----------------------------------------------------------------------------


void Shader::set_uniform(const char* name, float value)
{
    if (!pid_) return;
    int location = glGetUniformLocation(pid_, name);
    if (location == -1) {
        std::cerr<<"Invalid uniform location for: "<<name<<std::endl;
        return;
    }
    glUniform1f(location, value);
    return;
}


//-----------------------------------------------------------------------------


void Shader::set_uniform(const char* name, int value)
{
    if (!pid_) return;
    int location = glGetUniformLocation(pid_, name);
    if (location == -1){
        std::cerr<<"Invalid uniform location for: "<<name<<std::endl;
        glCheckError();
        return;
    }
    glUniform1i(location, value);
    return;
}


//-----------------------------------------------------------------------------


void Shader::set_uniform(const char* name, const vec3& vec)
{
    if (!pid_) return;
    int location = glGetUniformLocation(pid_, name);
    if(location == -1) {
        std::cerr<<"Invalid uniform location for: "<<name<<std::endl;
        return;
    };
    glUniform3f(location, vec[0], vec[1], vec[2]);
    return;
}


//-----------------------------------------------------------------------------


void Shader::set_uniform(const char* name, const vec4& vec)
{
    if (!pid_) return;
    int location = glGetUniformLocation(pid_, name);
    if(location == -1) {
        std::cerr<<"Invalid uniform location for: "<<name<<std::endl;
        return;
    }
    glUniform4f(location, vec[0], vec[1], vec[2], vec[3]);
    return;
}


//-----------------------------------------------------------------------------


void  Shader::set_uniform(const char* name, const mat3& mat)
{
    if (!pid_) return;
    int location = glGetUniformLocation(pid_, name);
    if(location == -1) {
        std::cerr<<"Invalid uniform location for: "<<name<<std::endl;
        return;
    }
    glUniformMatrix3fv(location, 1, false, mat.data());
    return;
}


//-----------------------------------------------------------------------------


void Shader::set_uniform(const char* name, const mat4& mat)
{
    if (!pid_) return;
    int location = glGetUniformLocation(pid_, name);
    if(location == -1){
        std::cerr<<"Invalid uniform location for: "<<name<<std::endl;
        return;
    }
    glUniformMatrix4fv(location, 1, false, mat.data());
    return;
}


//=============================================================================

