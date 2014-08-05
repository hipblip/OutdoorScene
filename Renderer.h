#pragma once

//struct GLFWwindow;

// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>
#include <GLM/glm.hpp>

#include "EventManager.h"

class Renderer {
public:
	static void setUp();
	static void terminateProgram();

	static void beginFrame();
	static void endFrame();

	static GLuint LoadShaders(const char * vertex_shader_path, const char * fragment_shader_path);
private:
	static GLFWwindow* window;
};