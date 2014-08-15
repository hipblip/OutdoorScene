#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

using namespace glm;

class GrassModel {

public:
	GrassModel();
	virtual ~GrassModel();

	virtual void Update( float dt );
	virtual void Draw();
private:
	GLuint vertexBufferID;
	int getRandomSign();
};

struct Vertex {
	vec3 position;
	vec3 normal;
	vec3 color;
};