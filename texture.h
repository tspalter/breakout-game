/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: header for Texture2D class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

// Texture2D class to store a texture
// plus utility functions
class Texture2D {
public:
	unsigned int ID;
	unsigned int width, height;

	// texture formatting
	unsigned int internalFormat; //texture object format
	unsigned int imageFormat; // loaded image format

	// texture configuration
	unsigned int wrapS; // wrap on S axis
	unsigned int wrapT; // wrap on T axis
	unsigned int filterMin; // filtering modes
	unsigned int filterMax;

	Texture2D();

	// generate texture from image data
	void Generate(unsigned int _width, unsigned int _height, unsigned char* data);
	// bind texture as current active GL_TEXTURE_2D texture object
	void Bind() const;
};

#endif