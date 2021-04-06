/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: header for PowerUp class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#ifndef POWER_UP_H
#define POWER_UP_H
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gameObject.h"

const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

// class for a PowerUp game object
class PowerUp : public GameObject {
public:
	// powerup state
	std::string type;
	float duration;
	bool activated;

	PowerUp(std::string _type, glm::vec3 color, float _duration, glm::vec2 _position, Texture2D texture)
		: GameObject(_position, POWERUP_SIZE, texture, color, VELOCITY), type(_type), duration(_duration), activated() { }
};

#endif