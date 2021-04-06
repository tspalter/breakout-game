/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: header for GameObject class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite.h"

class GameObject {
public:
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	float rotation;
	bool isSolid;
	bool Destroyed;

	// render state
	Texture2D sprite;

	GameObject();
	GameObject(glm::vec2 _pos, glm::vec2 _size, Texture2D _sprite, glm::vec3 _color = glm::vec3(1.0f), glm::vec2 _velocity = glm::vec2(0.0f, 0.0f));

	// draw sprite
	virtual void Draw(SpriteRenderer& renderer);
};

#endif