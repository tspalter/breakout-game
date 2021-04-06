/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: header for Game class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#pragma once
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gameObject.h"
#include "gameLevel.h"
#include "ball.h"
#include "powerup.h"

// Game States
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// Directions
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

// Game class holds all game-states and functionality
// Provides easy access to components and handles collisions
class Game {
public:
	GameState state;
	bool keys[1024];
	bool keysProcessed[1024];
	unsigned int width, height;
	std::vector<GameLevel> levels;
	std::vector<PowerUp> powerUps;
	unsigned int level;
	unsigned int lives;

	Game(unsigned int _width, unsigned int _height);
	~Game();

	// initialize game state (load shaders/textures/levels)
	void Init();

	// game loop functions
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	void EnemyMovement(float dt);
	
	bool CheckCollision(GameObject& one, GameObject& two);  // checks for AABB-AABB collision
	Collision CheckCollision(BallObject& one, GameObject& two);  // checks for circle-AABB collision
	void DoCollisions();

	Direction VectorDirection(glm::vec2 target);

	// reset functions
	void ResetLevel();
	void ResetPlayer();

	// power up functions
	bool ShouldSpawn(unsigned int chance);
	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(float dt);
	void ActivatePowerUp(PowerUp& powerUp);
	bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type);
};

#endif