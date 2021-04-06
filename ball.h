/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: header for ball class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/
#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gameObject.h"
#include "texture.h"

class BallObject : public GameObject {
public:
	// ball state
	float radius;
	bool stuck;
	bool sticky, passthrough;

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

	// move ball and keep it within constraints of window
	glm::vec2 Move(float dt, unsigned int windowWidth);

	// reset ball to original state
	void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif