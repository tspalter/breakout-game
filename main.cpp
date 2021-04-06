/* Start Header -------------------------------------------------------
	Copyright (C) 2020 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
	File Name: main.cpp
	Purpose: main source file for game engine
	Language: C++ MinGW
	Platform: Visual Studio for Windows 10
	Project: CS529_t.spalter_final
	Author: Thomas Spalter, t.spalter
	Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.h"
#include "resourceManager.h"

#include <iostream>

// GLFW func declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const unsigned int FRAME_RATE = 120;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);



int main(int argc, char* argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Breakout EFFECT", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// initialize game
	Breakout.Init();

	// deltaTime vars
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		// calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// manage user input
		Breakout.ProcessInput(deltaTime);

		// update game state
		Breakout.Update(deltaTime);

		// render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Breakout.Render();

		glfwSwapBuffers(window);
	}

	// delete all resources loaded with the ResourceManager
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// for pressing escape key, set WindowShouldClose to true
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			Breakout.keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			Breakout.keys[key] = false;
			Breakout.keysProcessed[key] = false;
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure viewport matches window dimensions
	glViewport(0, 0, width, height);
}