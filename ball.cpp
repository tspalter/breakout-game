/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: source for Ball class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#include "ball.h"

BallObject::BallObject() : GameObject(), radius(12.5f), stuck(true), sticky(false), passthrough(false) {

}

BallObject::BallObject(glm::vec2 pos, float _radius, glm::vec2 velocity, Texture2D sprite) 
	: GameObject(pos, glm::vec2(_radius*2.0f, _radius * 2.0f), sprite, glm::vec3(1.0), velocity), radius(_radius), stuck(true), sticky(false), passthrough(false) {

}

glm::vec2 BallObject::Move(float dt, unsigned int windowWidth) {
	// if not stuck to paddle
	if (!this->stuck) {
		// move the ball
		this->position += this->velocity * dt;

		// out of bounds check
		if (this->position.x <= 0.0f) {
			this->velocity.x = -this->velocity.x;
			this->position.x = 0.0f;
		}
		else if (this->position.x + this->size.x >= windowWidth) {
			this->velocity.x = -this->velocity.x;
			this->position.x = windowWidth - this->size.x;
		}
		if (this->position.y <= 0.0f) {
			this->velocity.y = -this->velocity.y;
			this->position.y = 0.0f;
		}
	}
	return this->position;
}

// reset ball to initial stuck position
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
	this->position = position;
	this->velocity = velocity;
	this->stuck = true;
	this->sticky = false;
	this->passthrough = false;
}