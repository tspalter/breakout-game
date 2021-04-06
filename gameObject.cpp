/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: source for GameObject class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#include "gameObject.h"

GameObject::GameObject() : position(0.0f, 0.0f), size(1.0f, 1.0f), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), isSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec2 _pos, glm::vec2 _size, Texture2D _sprite, glm::vec3 _color, glm::vec2 _velocity)
	: position(_pos), size(_size), velocity(_velocity), color(_color), rotation(0.0f), sprite(_sprite), isSolid(false), Destroyed(false) { }

void GameObject::Draw(SpriteRenderer& renderer) {
	renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}