#pragma once

#include <glm\glm.hpp>

using namespace glm;

struct Particle{
	vec3 position;
	vec3 movement;
	vec4 color;
	float size;
	float rotation;
	float age;
	float lifetime;
	float cameraDistance;

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameraDistance > that.cameraDistance;
	}
};