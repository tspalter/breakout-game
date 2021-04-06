/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: source for ResourceManager class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#include "resourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// instantiate static vars
std::map<std::string, Texture2D>	ResourceManager::textures;
std::map<std::string, Shader>		ResourceManager::shaders;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name) {
	shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return shaders[name];
}

Shader& ResourceManager::GetShader(std::string name) {
	return shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name) {
	textures[name] = loadTextureFromFile(file, alpha);
	return textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name) {
	return textures[name];
}

void ResourceManager::Clear() {
	// delete shaders
	for (auto s : shaders) {
		glDeleteProgram(s.second.ID);
	}

	// delete textures
	for (auto t : textures) {
		glDeleteTextures(1, &t.second.ID);
	}
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile) {
	// retrieve vertex/fragment source from filepath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	try {
		// open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;

		// read file buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// convert stream to string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// in case of geometry shader:
		if (gShaderFile != nullptr) {
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e) {
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	// create shader object from source
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
	// create texture object
	Texture2D texture;
	if (alpha) {
		texture.internalFormat = GL_RGBA;
	}

	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	// generate texture
	texture.Generate(width, height, data);

	// free image data
	stbi_image_free(data);
	return texture;
}