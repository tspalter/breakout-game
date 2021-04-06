/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: header for gameLevel class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gameObject.h"
#include "sprite.h"
#include "resourceManager.h"

// GameLevel class to hold all tiles within a level of the game
class GameLevel {
public:
	// level state
	std::vector<GameObject> bricks;

	GameLevel() { }

	// load level from file
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	void Draw(SpriteRenderer& renderer);

	// check that level is completed
	bool isCompleted();

private:
	// initialize level from data
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);

};


#endif