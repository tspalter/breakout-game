/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: text.cpp
	Purpose: source for Text class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "text.h"
#include "ResourceManager.h"

TextRenderer::TextRenderer(unsigned int width, unsigned int height) {
	// load and configure shader
	this->textShader = ResourceManager::LoadShader("text.vert", "text.frag", nullptr, "text");
	this->textShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f), true);
	this->textShader.SetInteger("text", 0);

	// configure VAO/VBO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, unsigned int fontSize) {
	this->characters.clear();

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// pre-load first 128 ASCII chars
	for (GLubyte c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);

		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// store char for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// destroy FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color) {
	// activate render state
	this->textShader.Use();
	this->textShader.SetVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	// iterate through chars
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = characters[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = y + (this->characters['H'].bearing.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		// update VBO
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 0.0f },

			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f }
		};

		// render glyph texture
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// update VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// advance cursors for next glyph
		x += (ch.advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}