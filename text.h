/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: text.h
	Purpose: header for TextRenderer class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"

// holds relevant info for a character
struct Character {
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

// class for rendering text displayed by a font
class TextRenderer {
public:
	// list of precompiled chars
	std::map<char, Character> characters;
	Shader textShader;

	TextRenderer(unsigned int width, unsigned int height);
	void Load(std::string font, unsigned int fontSize);
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:
	// render state
	unsigned int VAO, VBO;
};

#endif