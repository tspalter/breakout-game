/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: particle.cpp
	Purpose: source for ParticleGenerator class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#include "particle.h"

ParticleGenerator::ParticleGenerator(Shader& _shader, Texture2D& _texture, unsigned int _amount)
	: shader(_shader), texture(_texture), amount(_amount) {
	this->init();
}

void ParticleGenerator::Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset) {
	// add new particles
	for (unsigned int i = 0; i < newParticles; i++) {
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}

	// update all particles
	for (unsigned int i = 0; i < this->amount; i++) {
		Particle& p = this->particles[i];
		p.life -= dt; // reduce life
		if (p.life > 0.0f) { // particle is alive, update
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5f;
		}
	}
}

// render all particles
void ParticleGenerator::Draw() {
	// use additive blending to create glow effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for (Particle p : this->particles) {
		if (p.life > 0.0f) {
			this->shader.SetVector2f("offset", p.position);
			this->shader.SetVector4f("color", p.color);
			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	// reset to default blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
	unsigned int VBO;
	float particleQuad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);

	// fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);
	
	// set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);
	glBindVertexArray(0);

	// create amount default particle instances
	for (unsigned int i = 0; i < this->amount; i++) {
		this->particles.push_back(Particle());
	}
}

unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle() {
	// first search from lastUsedParticle
	for (unsigned int i = lastUsedParticle; i < this->amount; i++) {
		if (this->particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	// otherwise, linear search
	for (unsigned int i = 0; i < lastUsedParticle; i++) {
		if (this->particles[i].life <= 0.0) {
			lastUsedParticle = i;
			return i;
		}
	}

	// all particles taken, override first one
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset) {
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.position = object.position + random + offset;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.life = 1.0f;
	particle.velocity = object.velocity * 0.1f;
}