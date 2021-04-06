/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: particle.h
	Purpose: header for Particle class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "GameObject.h"

// struct to represent a particle and its state
struct Particle {
	glm::vec2 position, velocity;
	glm::vec4 color;
	float life;

	Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};

// container for rendering a large number of particles
class ParticleGenerator {
public:
	ParticleGenerator(Shader& _shader, Texture2D& _texture, unsigned int _amount);

	void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void Draw();
private:
	std::vector<Particle> particles;
	unsigned int amount;

	// render state
	Shader shader;
	Texture2D texture;
	unsigned int VAO;

	void init();

	// returns first Particle index that's currently unused
	unsigned int firstUnusedParticle();

	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif