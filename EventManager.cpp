/*
*	This class sets up openGL and terminates the program.  It also handles user input from the mouse as well as count the frametime.
*/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW - OpenGL Extension Wrangler
//#define GLEW_STATIC
//#include <GL/glew.h>

// Include GLFW - OpenGL Framework (Multi Platform framework to Setup a window and OpenGL Context
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM - OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EventManager.h"
using namespace glm;

// Window variable and dimensions
GLFWwindow* EventManager::window = NULL;
int EventManager::windowWidth = 1024;
int EventManager::windowHeight = 768;

// Frame time
float EventManager::sLastFrameTime = glfwGetTime();
float  EventManager::sFrameTime = 0.0f;

// Mouse
float EventManager::sLastMousePositionX = 0.0f;
float  EventManager::sMouseDeltaX = 0.0f;
float EventManager::sLastMousePositionY = 0.0f;
float  EventManager::sMouseDeltaY = 0.0f;

// Initializes glfw as well as the window
void EventManager::setUp() {

	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, 0);
	window = glfwCreateWindow(windowWidth, windowHeight, "Outdoor Scene", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open the window\n");
		glfwTerminate();
		exit(-1);
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

//Terminates glfw and window
void EventManager::terminateProgram() {
	glfwTerminate();
	window = NULL;
}

// When the user hits escape, the window closes
bool EventManager::exitLoop() {
	return (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window));
}

// Returns the window
GLFWwindow* EventManager::getWindow() {
	return window;
}

// Used to get the frame time
void EventManager::update() {
	// Update inputs / events
	glfwPollEvents();

	// Update mouse position
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	sMouseDeltaX = static_cast<float>( x - sLastMousePositionX );
	sMouseDeltaY = static_cast<float>( y - sLastMousePositionY );
	sLastMousePositionX = x;
	sLastMousePositionY = y;

	// Update frame time
	double currentTime = glfwGetTime();
	sFrameTime = static_cast<float>(currentTime - sLastFrameTime);
	sLastFrameTime = currentTime;
}

float EventManager::getFrameTime() {
	return sFrameTime;
}

float EventManager::getMouseMovementX() {
	return sMouseDeltaX;
}

float EventManager::getMouseMovementY() {
	return sMouseDeltaY;
}
