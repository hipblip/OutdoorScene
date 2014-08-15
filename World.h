#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>

#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include <GLFW/glfw3.h>

#include <vector>

#include "Renderer.h"
#include "Model.h"

class Model;

class World {
public:
	World();
	~World();

	void update(float dt);
	void draw();

	void addModel(Model* toAdd);
private:
	std::vector<Model*> models;

	GLuint vertexBufferID;
	GLuint vertexArrayID;
	unsigned int numVertices;
};