/* Start Header -------------------------------------------------------
    Copyright (C) 2020 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
    File Name: ball.h
    Purpose: header for Shader class
    Language: C++ MinGW
    Platform: Visual Studio for Windows 10
    Project: CS529_t.spalter_final
    Author: Thomas Spalter, t.spalter
    Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#ifndef SHADER_H
#define SHADER_H

#include <string>

#define GLEW_STATIC
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// General purpose shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader
{
public:
    unsigned int ID;

    Shader() : ID() { }

    // sets the current shader as active
    Shader& Use();

    // compiles the shader from given source code
    void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); // note: geometry source code is optional

    // utility functions
    void SetFloat(const char* name, float value, bool useShader = false);
    void SetInteger(const char* name, int value, bool useShader = false);
    void SetVector2f(const char* name, float x, float y, bool useShader = false);
    void SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
    void SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
    void SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
    void SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
    void SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
    void SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

private:
    // checks if compilation or linking failed and if so, print the error logs
    void checkCompileErrors(unsigned int object, std::string type);
};

#endif