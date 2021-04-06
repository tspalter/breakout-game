/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: ball.h
	Purpose: source for GameLevel class
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#include "gameLevel.h"

#include <fstream>
#include <sstream>

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight) {
	// clear old data
	this->bricks.clear();
	
	// load
	unsigned int tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;
	if (fstream) {
		while (std::getline(fstream, line)) {
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode) {
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		if (tileData.size() > 0) {
			this->init(tileData, levelWidth, levelHeight);
		}
	}
}

void GameLevel::Draw(SpriteRenderer& renderer) {
	for (GameObject& tile : this->bricks) {
		if (!tile.Destroyed) {
			tile.Draw(renderer);
		}
	}
}

bool GameLevel::isCompleted() {
	for (GameObject& tile : this->bricks) {
		if (!tile.isSolid && !tile.Destroyed) {
			return false;
		}
	}
	return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {
	// calculate dimensions
	unsigned int height = tileData.size();

	// initialize level tiles from tileData
	for (unsigned int y = 0; y < height; y++) {
		unsigned int width = tileData[y].size();
		float unitWidth = levelWidth / static_cast<float>(width), unitHeight = levelHeight / height;
		for (unsigned int x = 0; x < width; x++) {
			// check block type
			if (tileData[y][x] == 1) { // solid block, can't be broken
				glm::vec2 pos(unitWidth * x, unitHeight * y);
				glm::vec2 size(unitWidth, unitHeight);
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.isSolid = true;
				this->bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) { // non-solid, determine color
				glm::vec3 color = glm::vec3(1.0f); // default white
				if (tileData[y][x] == 2) {
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (tileData[y][x] == 3) {
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (tileData[y][x] == 4) {
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (tileData[y][x] == 5) {
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				}
				glm::vec2 pos(unitWidth * x, unitHeight * y);
				glm::vec2 size(unitWidth, unitHeight);
				GameObject obj(pos, size, ResourceManager::GetTexture("block"), color);
				this->bricks.push_back(obj);
			}
		}
	}
}