/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: header for ResourceManager class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

// static class that hosts several functions to load
// textures and shaders, stored using string handles
// for future use
class ResourceManager {
public:
	// resource storage
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;

	// shader functions
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	static Shader& GetShader(std::string name);

	// texture functions
	static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
	static Texture2D& GetTexture(std::string name);

	// deallocate all loaded resources
	static void Clear();

private:
	ResourceManager() { }

	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const char* file, bool alpha);
};

#endif

