/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: source for Texture2D class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#include "texture.h"

#include <iostream>

Texture2D::Texture2D() : width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB),
	wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_LINEAR), filterMax(GL_LINEAR) {
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(unsigned int _width, unsigned int _height, unsigned char* data) {
	this->width = _width;
	this->height = _height;

	// create texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);

	// set texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
	glBindTexture(GL_TEXTURE_2D, this->ID);
}