/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: header for PostProcessor class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite.h"
#include "shader.h"

// Handles all PostProcessing effects
class PostProcessor {
public:
	Shader postProcessingShader;
	Texture2D texture;
	unsigned int width, height;

	// types of effects
	bool confuse, chaos, shake;

	PostProcessor(Shader shader, unsigned int width, unsigned int height);
	
	// prep postprocessor's frame buffer operations
	void BeginRender();
	// called after rendering game, stores render data into texture object
	void EndRender();
	// renders postprocessor texture quad
	void Render(float time);
	
private:
	// render state
	unsigned int MSFBO, FBO; // MS is multisampled
	unsigned int RBO; // color buffer
	unsigned int VAO;
	// initialize quad
	void initRenderData();
};

#endif