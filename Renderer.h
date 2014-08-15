#pragma once

//struct GLFWwindow;

// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

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

	static GLuint getShader();
	static void setShader(GLuint value);

	static GLuint getShaderVectorPosition() { return currentShader; };
private:
	static GLFWwindow* window;

	//static unsigned int programID;

	static std::vector<GLuint> Renderer::shaderProgramID;
	static GLuint Renderer::currentShader;
};